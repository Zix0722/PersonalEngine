#include "Engine/Math/IntRange .hpp"

const IntRange IntRange::ZERO = IntRange(0, 0);
const IntRange IntRange::ONE = IntRange(1, 1);
const IntRange IntRange::ZERO_TO_ONE = IntRange(0, 1);

IntRange::IntRange(int min, int max)
	:m_min(min)
	,m_max(max)
{

}

bool IntRange::IsOnRange(int value) const
{
	if (value > m_min && value < m_max)
	{
		return true;
	}
	return false;
}

bool IntRange::IsOverlappingWith(IntRange const& otherRange) const
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

bool IntRange::operator!=(IntRange const& compare) const
{
	if (m_max == compare.m_max && m_min == compare.m_min)
	{
		return false;
	}
	return true;
}

bool IntRange::operator==(IntRange const& compare) const
{
	if (m_max == compare.m_max && m_min == compare.m_min)
	{
		return true;
	}
	return false;
}

void IntRange::operator=(IntRange const& copyFrom)
{
	m_max = copyFrom.m_max;
	m_min = copyFrom.m_min;
}
