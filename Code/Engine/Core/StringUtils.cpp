#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <string>
#include "ErrorWarningAssert.hpp"


//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;

bool isWhitespaceOrEmpty(const std::string& str) {
	for (char c : str) {
		if (!std::isspace(static_cast<unsigned char>(c))) {
			return false; // Found a non-whitespace character
		}
	}
	return true; // String is either empty or contains only whitespace
}

Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn, bool ignoreEmptyMode)
{
	Strings delimatedStrings;

	int delimeterPos = -1;
	int subStringIndex = 0;
	int subStringStartPos = 0;
	std::string subStr;

	delimeterPos = (int)originalString.find(delimiterToSplitOn, subStringIndex);
	subStringStartPos = delimeterPos;

	while (delimeterPos != std::string::npos)
	{
		subStr = originalString.substr(subStringIndex, subStringStartPos);

		if (ignoreEmptyMode)
		{
			if (isWhitespaceOrEmpty(subStr))
			{
				subStringIndex = delimeterPos + 1;
				delimeterPos = (int)originalString.find(delimiterToSplitOn, subStringIndex);
				subStringStartPos = delimeterPos - subStringIndex;
			}
			else
			{
				delimatedStrings.push_back(subStr);
				subStringIndex = delimeterPos + 1;
				delimeterPos = (int)originalString.find(delimiterToSplitOn, subStringIndex);
				subStringStartPos = delimeterPos - subStringIndex;
			}
		}
		else
		{
			delimatedStrings.push_back(subStr);
			subStringIndex = delimeterPos + 1;
			delimeterPos = (int)originalString.find(delimiterToSplitOn, subStringIndex);
			subStringStartPos = delimeterPos - subStringIndex;
		}

		
	}

	subStr = originalString.substr(subStringIndex, delimeterPos);
	if (ignoreEmptyMode)
	{
		if (isWhitespaceOrEmpty(subStr))
		{

		}
		else
		{
			delimatedStrings.push_back(subStr);
		}
	}
	else
	{
		delimatedStrings.push_back(subStr);
	}

	return delimatedStrings;

}

Strings SplitStringWithQuotes(const std::string& originalString, char delimiterToSplitOn)
{
	Strings result;
	std::string token;
	bool insideQuotes = false;

	for (char c : originalString) {
		if (c == '"') {
			insideQuotes = !insideQuotes;
			token += c;
		}
		else if (c == delimiterToSplitOn && !insideQuotes) {
			result.push_back(token);
			token.clear();
		}
		else {
			token += c;
		}
	}

	result.push_back(token);  // Add the last token

	return result;
}

void TrimString(std::string& originalString, char delimiterToTrim)
{
	int delimeterPos = -1;
	int subStringIndex = 0;
	int subStringStartPos = 0;
	std::string subStr;

	delimeterPos = (int)originalString.find(delimiterToTrim, subStringIndex);
	subStringStartPos = delimeterPos;

	while (delimeterPos != std::string::npos)
	{
		subStr = originalString.substr(subStringIndex, subStringStartPos);

		if (isWhitespaceOrEmpty(subStr))
		{
			subStringIndex = delimeterPos + 1;
			delimeterPos = (int)originalString.find(delimiterToTrim, subStringIndex);
			subStringStartPos = delimeterPos - subStringIndex;
		}
		else
		{
			originalString = subStr;
			break;
		}
	}
}



bool GetBoolFromText(char const* text)
{
	std::string textString = text;
	if (textString.compare("false") == 0)
	{
		return false;
	}
	else if (textString.compare("true") == 0)
	{
		return true;
	}
	else
	{
		std::string errorMessage = std::string("Failed to convert: ") + text + std::string("to bool");
		ERROR_AND_DIE(errorMessage.c_str());
	}
}

//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( int maxLength, char const* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}





