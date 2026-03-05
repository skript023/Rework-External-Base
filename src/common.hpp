#pragma once
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include <d3d11.h>
#include <time.h>
#include <vector>
#include <thread>
#include <stack>
#include <array>

#include <chrono>
#include <atomic>
#include <optional>
#include <filesystem>
#include <fstream>
#include <map>

#include <regex>

#include <xinput.h>
#include <nlohmann/json.hpp>

#include "core.hpp"

#include <logger/logger.hpp>
#include <menu_settings.hpp>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Xinput.lib")

namespace ellohim
{
	using namespace std::chrono_literals;
	inline std::atomic_bool g_running{ true };
}