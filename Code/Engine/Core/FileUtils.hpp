#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include <string>
#include <vector>

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename);
int FileReadToString(std::string& outString, std::string const& filename);

