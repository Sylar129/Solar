#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include "Core/Base/Log.h"
#include "Core/Debug/Instrumentor.h"

#ifdef SOLAR_PLATFORM_WINDOWS
#include <Windows.h>
#endif
