// Copyright (c) 2024 Sylar129

#pragma once

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "Core/Base/Log.h"

namespace solar {

using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct ProfileResult {
  std::string name;

  FloatingPointMicroseconds start;
  std::chrono::microseconds elapsed_time;
  std::thread::id thread_id;
};

struct InstrumentationSession {
  std::string name;
};

class Instrumentor {
 public:
  Instrumentor(const Instrumentor&) = delete;
  Instrumentor(Instrumentor&&) = delete;

  void BeginSession(const std::string& name,
                    const std::string& filepath = "results.json") {
    std::lock_guard lock(mutex_);
    if (current_session_ != nullptr) {
      // If there is already a current session, then close it before
      // beginning new one. Subsequent profiling output meant for the
      // original session will end up in the newly opened session instead.
      // That's better than having badly formatted profiling output.
      if (Log::GetCoreLogger()) {  // Edge case: BeginSession() might be
                                   // before Log::Init()
        SOLAR_CORE_ERROR(
            "Instrumentor::BeginSession('{0}') when "
            "session '{1}' already open.",
            name, current_session_->name);
      }
      InternalEndSession();
    }

    output_stream_.open(filepath);

    if (output_stream_.is_open()) {
      current_session_ = new InstrumentationSession({name});
      WriteHeader();
    } else {
      if (Log::GetCoreLogger()) {  // Edge case: BeginSession() might be
                                   // before Log::Init()
        SOLAR_CORE_ERROR("Instrumentor could not open results file '{0}'.",
                         filepath);
      }
    }
  }

  void EndSession() {
    std::lock_guard lock(mutex_);
    InternalEndSession();
  }

  void WriteProfile(const ProfileResult& result) {
    std::stringstream json;

    json << std::setprecision(3) << std::fixed;
    output_stream_ << ",{";
    output_stream_ << "\"cat\":\"function\",";
    output_stream_ << "\"dur\":" << (result.elapsed_time.count()) << ',';
    output_stream_ << "\"name\":\"" << result.name << "\",";
    output_stream_ << "\"ph\":\"X\",";
    output_stream_ << "\"pid\":0,";
    output_stream_ << "\"tid\":" << result.thread_id << ",";
    output_stream_ << "\"ts\":" << result.start.count();
    output_stream_ << "}";

    std::lock_guard lock(mutex_);
    if (current_session_ != nullptr) {
      output_stream_ << json.str();
      output_stream_.flush();
    }
  }

  static Instrumentor& Get() {
    static Instrumentor s_instance;
    return s_instance;
  }

 private:
  Instrumentor() : current_session_(nullptr) {}

  ~Instrumentor() { EndSession(); }

  void WriteHeader() {
    output_stream_ << "{\"otherData\": {},\"traceEvents\":[{}";
    output_stream_.flush();
  }

  void WriteFooter() {
    output_stream_ << "]}";
    output_stream_.flush();
  }

  // Note: you must already own lock on m_Mutex before
  // calling InternalEndSession()
  void InternalEndSession() {
    if (current_session_ != nullptr) {
      WriteFooter();
      output_stream_.close();
      delete current_session_;
      current_session_ = nullptr;
    }
  }

  std::mutex mutex_;
  InstrumentationSession* current_session_;
  std::ofstream output_stream_;
};

class InstrumentationTimer {
 public:
  explicit InstrumentationTimer(const char* name)
      : name_(name), stopped_(false) {
    start_timepoint_ = std::chrono::high_resolution_clock::now();
  }

  ~InstrumentationTimer() {
    if (!stopped_) {
      Stop();
    }
  }

  void Stop() {
    auto end_timepoint = std::chrono::high_resolution_clock::now();
    auto high_res_start =
        FloatingPointMicroseconds{start_timepoint_.time_since_epoch()};
    auto elapsed_time =
        std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint)
            .time_since_epoch() -
        std::chrono::time_point_cast<std::chrono::microseconds>(
            start_timepoint_)
            .time_since_epoch();

    Instrumentor::Get().WriteProfile(
        {name_, high_res_start, elapsed_time, std::this_thread::get_id()});

    stopped_ = true;
  }

 private:
  const char* name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;
  bool stopped_;
};

}  // namespace solar

#define SOLAR_PROFILE 0

#if SOLAR_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define SOLAR_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define SOLAR_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define SOLAR_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define SOLAR_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define SOLAR_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define SOLAR_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define SOLAR_FUNC_SIG __func__
#else
#define SOLAR_FUNC_SIG "SOLAR_FUNC_SIG unknown!"
#endif

#define SOLAR_PROFILE_BEGIN_SESSION(name, filepath) \
  ::Solar::Instrumentor::Get().BeginSession(name, filepath)
#define SOLAR_PROFILE_END_SESSION() ::Solar::Instrumentor::Get().EndSession()
#define SOLAR_PROFILE_SCOPE(name) \
  ::Solar::InstrumentationTimer timer##__LINE__(name);
#define SOLAR_PROFILE_FUNCTION() SOLAR_PROFILE_SCOPE(SOLAR_FUNC_SIG)
#else
#define SOLAR_PROFILE_BEGIN_SESSION(name, filepath)
#define SOLAR_PROFILE_END_SESSION()
#define SOLAR_PROFILE_SCOPE(name)
#define SOLAR_PROFILE_FUNCTION()
#endif
