#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <variant>

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <limits>
#include <mutex>
#include <future>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

//#include <D3d12.h>

// Soren+Neural

// Activation
#include "Activation/Activation.h"

// ThreadPool
#include "ThreadPool.h"

// Timer
#include "Timer.h"

// Random
#include "Random/Random.h"

// Log
#include "Log/Log.h"

// JSON
#include "nlohmann/json.hpp"


#ifdef SOREN_PLATFORM_WINDOWS
#include <Windows.h>
#endif // SOREN_PLATFORM_WINDOWS
