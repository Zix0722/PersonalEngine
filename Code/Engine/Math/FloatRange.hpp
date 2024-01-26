#pragma once

struct FloatRange
{
public:
	float m_min = 0.f;
	float m_max = 0.f;

	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;

public:
	FloatRange() = default;
	explicit FloatRange(float min, float max);

	//Accessors
	bool		IsOnRange(float value) const;
	bool		IsOverlappingWith(FloatRange const& otherRange) const;

	// -------------------------------------------------------
	void		operator=(FloatRange const& copyFrom);
	bool		operator==(FloatRange const& compare) const;
	bool		operator!=(FloatRange const& compare) const;
}; 
