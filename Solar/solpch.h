// Copyright (c) 2024 Sylar129

#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "core/base/log.h"
#include "core/debug/instrumentor.h"
#include "glm/glm.hpp"

#ifdef SOLAR_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif
