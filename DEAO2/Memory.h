// Author: Deci | Project: DEAO2 | Name: Memory.h
// Date: 2024/08/19 @ 12:08:05

#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <format>
#include "Logging.h"
#include <vector>

const inline std::string DE_EXE_NAME = "DOOMEternalx64vk.exe";

DWORD64           ModulePatternScan(std::string scanFriendlyName, const char* signature);
DWORD64           PatternScan(const char* szModule, const char* signature);
std::vector<char> PatternToByte(const char* pattern);
