#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

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

#include "Activation/Activation.h"
#include "ThreadPool.h"
#include "Random/Random.h"
#include "Log/Log.h"

#include "nlohmann/json.hpp"


#ifdef OL_PLATFORM_WINDOWS
#include <Windows.h>
#endif // OL_PLATFORM_WINDOWS
