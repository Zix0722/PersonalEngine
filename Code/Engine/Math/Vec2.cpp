#define _USE_MATH_DEFINES
#include "Engine/Math/Vec2.hpp"
#include <math.h>
#include "Engine/Math/MathUtils.hpp"
#include "../Core/StringUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"
#include "Vec3.hpp"
//#include "Engine/Math/MathUtils.hpp"
//#include "Engine/Core/EngineCommon.hpp"

const Vec2 Vec2::ZERO = Vec2(0.f, 0.f);
const Vec2 Vec2::ONE = Vec2(1.f, 1.f);
//-----------------------------------------------------------------------------------------------
Vec2::Vec2( const Vec2& copy )
	: x( copy.x )
	, y( copy.y )
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}

Vec2::Vec2(Vec3 vec3)
	: x(vec3.x)
	, y(vec3.y)
{

}

Vec2 const Vec2::MakeFromPolarRadians(float orentationRadians, float length)
{
	Vec2 result;
	result.x = length * cosf(orentationRadians);
	result.y = length * sinf(orentationRadians);
	
	return result;
}

Vec2 const Vec2::MakeFromPolarDegrees(float orentationDegrees, float length)
{
	Vec2 result;
	result.x = length * cosf(orentationDegrees / static_cast<float>((180.f / M_PI)));
	result.y = length * sinf(orentationDegrees / static_cast<float>((180.f / M_PI)));

	return result;
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + (const Vec2& vecToAdd) const
{
	return Vec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-( const Vec2& vecToSubtract ) const
{
	return Vec2( x-vecToSubtract.x, y-vecToSubtract.y );
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const
{
	return Vec2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( float uniformScale ) const
{
	return Vec2( x*uniformScale, y*uniformScale );
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( const Vec2& vecToMultiply ) const
{
	return Vec2( x*vecToMultiply.x, y*vecToMultiply.y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/( float inverseScale ) const
{
	return Vec2( x/inverseScale, y/inverseScale );
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=( const Vec2& vecToAdd )
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=( const Vec2& vecToSubtract )
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=( const float uniformScale )
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=( const Vec2& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;

}

Vec2 Vec2::transformCartesianToPolar_Radians()
{
	float R = sqrtf(x * x + y * y);
	float radians = static_cast<float>(atan2(y,x));

	return Vec2(R,radians);
}

Vec2 Vec2::transformCartesianToPolar_Degrees()
{
	float R = sqrtf(x * x + y * y);
	float degrees = static_cast<float>( atan2(y, x) ) * static_cast<float>((180.f / M_PI));

	return Vec2(R, degrees);
}

//-----------------------------------------------------------------------------------------------
const Vec2 operator*( float uniformScale, const Vec2& vecToScale )
{
	return Vec2( uniformScale*vecToScale.x, uniformScale*vecToScale.y );
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==( const Vec2& compare ) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			return true;
		}
	}
	return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=( const Vec2& compare ) const
{
	if (x == compare.x) {
		if (y == compare.y) {
			return false;
		}
	}
	return true;
}

float Vec2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

float Vec2::GetLengthSquared() const
{
	return x * x + y * y;
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}

float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}

Vec2 const Vec2::GetRotated90Degrees() const
{
	return Vec2 (-y,x);
}

Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2 (y,-x);
}

Vec2 const Vec2::GetRotatedRadians(float deltaRadians) const
{
	float R = GetLength();
	float Theta = GetOrientationRadians() + deltaRadians; // rotated
	return Vec2 (R * cosf(Theta), R * sinf(Theta));
}

Vec2 const Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	float R = GetLength();
	float Theta = GetOrientationDegrees() + deltaDegrees; // rotated
	return Vec2(R * CosDegrees(Theta), R * SinDegrees(Theta));
}

Vec2 const Vec2::GetClamped(float maxLength) const
{
	float R = GetLength();
	float Theta = GetOrientationRadians(); 
	Vec2 clampedResult;
	if (maxLength < R)
	{
		clampedResult.x = maxLength * cosf(Theta);
		clampedResult.y = maxLength * sinf(Theta);
		return clampedResult;
	}
	else
	{
		return Vec2(x, y);              // to-do
	}

	
}

Vec2 const Vec2::GetNormalized() const
{
	float Theta = GetOrientationDegrees();
	Vec2 nomalizedResult;
	nomalizedResult.x = 1 * CosDegrees(Theta);
	nomalizedResult.y = 1 * SinDegrees(Theta);
	return nomalizedResult;
}


Vec2 const Vec2::GetReflected(Vec2 const& vectorNormal) const
{
	Vec2 valOnNormalDir = DotProduct2D(*this, vectorNormal) * vectorNormal;
	Vec2 valOnTangantDir = *this - valOnNormalDir;
	Vec2 resultVector = valOnTangantDir + (-valOnNormalDir);
	return resultVector;
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float R = GetLength();
	x = cosf(newOrientationRadians) * R;
	y = sinf(newOrientationRadians) * R;
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float R = GetLength();
	x = CosDegrees(newOrientationDegrees) * R;
	y = SinDegrees(newOrientationDegrees) * R;
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = cosf(newOrientationRadians) * newLength;
	y = sinf(newOrientationRadians) * newLength;
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	x = CosDegrees(newOrientationDegrees) * newLength;
	y = SinDegrees(newOrientationDegrees) * newLength;
}

void Vec2::Rotate90Degrees()
{
	float tempX = x;
	float tempY = y;
	x = -tempY;
	y = tempX;
}

void Vec2::RotateMinus90Degrees()
{
	float tempX = x;
	float tempY = y;
	x = tempY;
	y = -tempX;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float R = GetLength();
	float Theta = GetOrientationRadians() + deltaRadians; // rotated
	x = R * cosf(Theta);
	y = R * sinf(Theta);
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float R = GetLength();
	float Theta = GetOrientationDegrees() + deltaDegrees; // rotated
	x = R * CosDegrees(Theta);
	y = R * SinDegrees(Theta);
}

void Vec2::SetLength(float newLength)
{
	float Theta = GetOrientationRadians();
	x = cosf(Theta) * newLength;
	y = sinf(Theta) * newLength;
}

void Vec2::ClampLength(float maxLength)
{
	float R = GetLength();
	float Theta = GetOrientationRadians();
	if (maxLength < R)
	{
		x = maxLength * cosf(Theta);
		y = maxLength * sinf(Theta);
	}
}

void Vec2::Normalize()
{
	float Theta = GetOrientationDegrees();
	x = CosDegrees(Theta);
	y = SinDegrees(Theta);
}

void Vec2::Reflect(Vec2 const& vectorNormal)
{
	Vec2 valOnNormalDir = DotProduct2D(*this, vectorNormal) * vectorNormal;
	Vec2 valOnTangantDir = *this - valOnNormalDir;
	Vec2 resultVector = valOnTangantDir + (-valOnNormalDir);
	*this = resultVector;
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float previousLength = GetLength();
	Normalize();

	return previousLength;
}

void Vec2::SetFromText(char const* text)
{
	std::string strText = text;

	Strings splitStrings = SplitStringOnDelimiter(text, ',');
	GUARANTEE_OR_DIE(splitStrings.size() == 2, Stringf("Vec2 can't construct from improper string \"%s\"", text));

	x = (float)atof(splitStrings[0].c_str());
	y = (float)atof(splitStrings[1].c_str());
}
