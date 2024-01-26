#include <math.h>
#include "Engine/Math/Vec2.hpp"
#include "IntVec2.hpp"
#include "../Core/StringUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"


IntVec2::IntVec2(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

IntVec2::IntVec2(int initialX, int initialY)
{
	x = initialX;
	y = initialY;
}

float IntVec2::GetLength() const
{
	return sqrtf(static_cast<float>(x * x + y * y));
}

int IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}

int IntVec2::GetLengthSquared() const
{
	return x*x + y*y;
}

float IntVec2::GetOrientationRadians() const
{
	Vec2 result(static_cast<float>(x), static_cast<float>(y));
	return result.GetOrientationRadians();
}

float IntVec2::GetOrientationDegrees() const
{
	Vec2 result(static_cast<float>(x), static_cast<float>(y));
	return result.GetOrientationDegrees();
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{	
	return IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

void IntVec2::SetFromText(const char* text)
{
	std::string strText = text;

	Strings splitStrings = SplitStringOnDelimiter(text, ',');
	GUARANTEE_OR_DIE(splitStrings.size() == 2, Stringf("IntVec2 can't construct from improper string \"%s\"", text));

	x = atoi(splitStrings[0].c_str());
	y = atoi(splitStrings[1].c_str());
}

const IntVec2 IntVec2::operator+(const IntVec2& vecToAdd) const
{
	return IntVec2(x + vecToAdd.x, y + vecToAdd.y);
}

const IntVec2 IntVec2::operator-(const IntVec2& vecToSubtract) const
{
	return IntVec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

const IntVec2 IntVec2::operator-() const
{
	return IntVec2(-x, -y);
}

const IntVec2 IntVec2::operator*(int uniformScale) const
{
	return IntVec2(x * uniformScale, y * uniformScale);
}

const IntVec2 IntVec2::operator*(const IntVec2& vecToMultiply) const
{
	return IntVec2(x * vecToMultiply.x, y * vecToMultiply.y);

}

void IntVec2::operator+=(const IntVec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator-=(const IntVec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator=(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


bool IntVec2::operator!=(const IntVec2& compare) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			return false;
		}
	}
	return true;
}

bool IntVec2::operator==(const IntVec2& compare) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			return true;
		}
	}
	return false;
}

bool IntVec2::operator<(const IntVec2& comparewith) const
{
	return x < comparewith.x || (x == comparewith.x && y < comparewith.y);
}

void IntVec2::Rotate90Degrees()
{
	int tempX = x;
	x = -y;
	y = tempX;
}

void IntVec2::RotateMinus90Degrees()
{
	int tempX = x;
	x = y;
	y = -tempX;
}
