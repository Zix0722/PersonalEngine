#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "AnalogJoystick.hpp"


Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPostion;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPostion.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPostion.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
	return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
	m_rawPosition = Vec2(0.f, 0.f);
	m_correctedPostion = Vec2(0.f, 0.f);

}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadzoneThreshold, float normalizeOuterDeadzoneThreshold)
{
	m_innerDeadZoneFraction = normalizedInnerDeadzoneThreshold;
	m_outerDeadZoneFraction = normalizeOuterDeadzoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawnormalizedX, float rawNormalizedY)
{
	m_rawPosition.x = rawnormalizedX;
	m_rawPosition.y = rawNormalizedY;

	float rawMag = m_rawPosition.GetLength();
	float degrees = m_rawPosition.GetOrientationDegrees();
	float inBetweenMag = RangeMapClamped(rawMag, m_innerDeadZoneFraction, m_outerDeadZoneFraction, 0.f, 1.f);
	m_correctedPostion = Vec2::MakeFromPolarDegrees(degrees, inBetweenMag);
	
}
