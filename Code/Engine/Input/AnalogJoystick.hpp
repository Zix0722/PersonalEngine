#pragma once
#include "Engine/Math/Vec2.hpp"
class AnalogJoystick
{
public:
	Vec2	GetPosition() const;
	float	GetMagnitude() const;
	float	GetOrientationDegrees() const;

	Vec2	GetRawUncorrectedPosition() const;
	float	GetInnerDeadZoneFraction() const;
	float	GetOuterDeadZoneFraction() const;

	void	Reset();
	void	SetDeadZoneThresholds(float normalizedInnerDeadzoneThreshold, float normalizeOuterDeadzoneThreshold);
	void	UpdatePosition(float rawnormalizedX, float rawNormalizedY);

protected:
	Vec2	m_rawPosition;
	Vec2	m_correctedPostion;
	float	m_innerDeadZoneFraction = 0.30f;
	float	m_outerDeadZoneFraction = 0.95f;
};