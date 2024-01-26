#pragma once
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>

typedef std::vector< std::string >		Strings;
Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn, bool ignoreEmptyMode = false);
Strings SplitStringWithQuotes(const std::string& originalString, char delimiterToSplitOn);
void TrimString(std::string& originalString, char delimiterToTrim);
bool	GetBoolFromText(char const* text);

//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... );
const std::string Stringf( int maxLength, char const* format, ... );




