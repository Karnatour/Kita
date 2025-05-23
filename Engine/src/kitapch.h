#pragma once

#ifndef NOMINMAX
//github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif

#include <Windows.h>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <filesystem>
#include <chrono>
#include <ranges>

#include "Core/Logger.h"
#include "Core/FileReader.h"
