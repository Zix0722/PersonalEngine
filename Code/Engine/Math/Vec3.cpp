#define _USE_MATH_DEFINES
#include <math.h>
//-----------------------------------------todo

#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "../Core/StringUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"
//#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"


//-----------------------------------------------------------------------------------------------
Vec3::Vec3(const Vec3& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
{
}


//-----------------------------------------------------------------------------------------------
Vec3::Vec3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

float Vec3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vec3::GetLengthXY() const
{
	return sqrtf(x * x + y * y);
}

float Vec3::GetLengthSquared() const
{
	return x* x + y * y + z * z;
}

float Vec3::GetLengthXYSquared() const
{
	return x * x + y * y;
}

float Vec3::GetAngleAboutZRadians() const
{
	return static_cast<float>(atan2(y,x));
}

float Vec3::GetAngleAboutZDegrees() const
{
	return static_cast<float>(atan2(y, x)) * static_cast<float>((180.f / M_PI));
}

Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	Vec3 result;
	float R = GetLengthXY();
	float Theta = GetAngleAboutZRadians() + deltaRadians; // rotated

	result.x = R * cosf(Theta);
	result.y = R * sinf(Theta);
	result.z = z;

	return result;
}

Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	Vec3 result;
	float R = GetLengthXY();
	float Theta = GetAngleAboutZDegrees() + deltaDegrees; // rotated

	result.x = R * CosDegrees(Theta);
	result.y = R * SinDegrees(Theta);
	result.z = z;

	return result;
}

Vec3 const Vec3::GetClamped(float maxLength) const
{
	float R = GetLength();
	float Theta = GetAngleAboutZRadians();
	Vec3 result;

	if (R > maxLength)
	{
		result.x = maxLength * cosf(Theta);
		result.y = maxLength * sinf(Theta);
		result.z = z;
		return result;
	}
	else
	{
		return Vec3(x, y, z);
	}
}

Vec3 const Vec3::GetNormalized() const
{
	float R = GetLength();
	if (R != 0.f)
	{
		float lengthFactor = 1.f / R;
		return Vec3(x * lengthFactor, y * lengthFactor, z * lengthFactor);
	}
	else
	{
		return Vec3(0.f, 0.f, 0.f);
	}
}




Vec2 const Vec3::GetVec2() const
{
	return Vec2(x, y);
}

void Vec3::SetFromText(char const* text)
{
	std::string strText = text;

	Strings splitStrings = SplitStringOnDelimiter(text, ',');
	GUARANTEE_OR_DIE(splitStrings.size() == 3, Stringf("Vec2 can't construct from improper string \"%s\"", text));

	x = (float)atof(splitStrings[0].c_str());
	y = (float)atof(splitStrings[1].c_str());
	z = (float)atof(splitStrings[2].c_str());
}

Vec3 const Vec3::MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length /*= 1.f*/)
{
	float cosLatitude = cosf(latitudeRadians);
	float sinLatitude = sinf(latitudeRadians);
	float cosLongitude = cosf(longitudeRadians);
	float sinLongitude = sinf(longitudeRadians);
	float z = length * sinLatitude;
	float xy = length * cosLatitude;
	float x = xy * cosLongitude;
	float y = xy * sinLongitude;
	return Vec3(x, y, z);
}

Vec3 const Vec3::MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length /*= 1.f*/)
{

	float cosLatitude = CosDegrees(latitudeDegrees);
	float sinLatitude = SinDegrees(latitudeDegrees);
	float cosLongitude = CosDegrees(longitudeDegrees);
	float sinLongitude = SinDegrees(longitudeDegrees);
	float z = length * sinLatitude;
	float xy = length * cosLatitude;
	float x = xy * cosLongitude;
	float y = xy * sinLongitude;
	return Vec3(x, y, z);
}

// void Vec3::SetLength(float newLength)
// {
// 
// }

//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator + (const Vec3& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator-(const Vec3& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}


//------------------------------------------------------------------------------------------------
const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}


//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}


//------------------------------------------------------------------------------------------------
const Vec3 Vec3::operator*(const Vec3& vecToMultiply) const
{
	return Vec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
}


//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator/(float inverseScale) const
{
	return Vec3(x / inverseScale, y / inverseScale, z / inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator+=(const Vec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator-=(const Vec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator=(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}


//-----------------------------------------------------------------------------------------------
const Vec3 operator*(float uniformScale, const Vec3& vecToScale)
{
	return Vec3(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z);
}

Vec3 const MakeFromPolarDegrees(float orientationDegrees, float length)
{
	float orientationInRadians = ConvertDegreesToRadians(orientationDegrees);
	return Vec3(length * cosf(orientationInRadians), length * sinf(orientationInRadians), 0);
}

//-----------------------------------------------------------------------------------------------
bool Vec3::operator==(const Vec3& compare) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			if (z == compare.z) {
				return true;
			}
		}
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator!=(const Vec3& compare) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			if (z == compare.z) {
				return false;
			}
		}
	}
	return true;
}

