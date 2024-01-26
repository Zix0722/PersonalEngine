#include "Engine/Math/FloatRange.hpp"

const FloatRange FloatRange::ZERO = FloatRange(0.f, 0.f);
const FloatRange FloatRange::ONE = FloatRange(1.f, 1.f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.f, 1.f);

FloatRange::FloatRange(float min, float max)
	:m_min(min)
	,m_max(max)
{

}

bool FloatRange::IsOnRange(float value) const
{
	if (value > m_min && value < m_max)
	{
		return true;
	}
	return false;
}

bool FloatRange::IsOverlappingWith(FloatRange const& otherRange) const
{
	if (m_max >= otherRange.m_min)
	{
		return false;
	}
	if (m_min >= otherRange.m_max)
	{
		return false;
	}
	return true;
}

bool FloatRange::operator!=(FloatRange const& compare) const
{
	if (m_max == compare.m_max && m_min == compare.m_min)
	{
		return false;
	}
	return true;
}

bool FloatRange::operator==(FloatRange const& compare) const
{
	if (m_max == compare.m_max && m_min == compare.m_min)
	{
		return true;
	}
	return false;
}

void FloatRange::operator=(FloatRange const& copyFrom)
{
	m_max = copyFrom.m_max;
	m_min = copyFrom.m_min;
}
