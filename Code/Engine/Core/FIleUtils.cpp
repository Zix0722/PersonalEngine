#include "Engine/Core/FileUtils.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "ErrorWarningAssert.hpp"

int FileReadToBuffer(std::vector<uint8_t>& outBuffer, std::string const& filename)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename.c_str(), "rb");
	if (fp == nullptr)
	{
		DebuggerPrintf("fp is nullptr in this case, check your filename.");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);

	outBuffer.resize(fileSize);

	fseek(fp, 0, SEEK_SET);
	size_t size = fread(&outBuffer[0], 1, (size_t)fileSize, fp);

	fclose(fp);

	return static_cast<int>(size);
}

int FileReadToString(std::string& outString, std::string const& filename)
{
	std::vector<uint8_t> buffer;
	int flag = FileReadToBuffer(buffer, filename);
	if (flag == -1)
	{
		DebuggerPrintf("fp is nullptr in this case, check your filename.");
		return -1;
	}

	std::string str(buffer.begin(), buffer.end());
	outString = str;

	return 1;
}

