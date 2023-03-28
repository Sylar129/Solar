#pragma once

#include <chrono>

namespace Solar {

class Timer {
public:
    using Clock = std::chrono::high_resolution_clock;
    using Unit = std::chrono::nanoseconds;
    Timer() { Reset(); }

    void Reset() { m_Start = Clock::now(); }

    float Elapsed()
    {
        return std::chrono::duration_cast<Unit>(Clock::now() - m_Start)
                   .count() *
               0.001f * 0.001f * 0.001f;
    }

    float ElapsedMillis() { return Elapsed() * 1000.0f; }

private:
    std::chrono::time_point<Clock> m_Start;
};

} // namespace Solar
