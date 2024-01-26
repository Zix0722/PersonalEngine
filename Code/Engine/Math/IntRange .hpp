#pragma once

struct IntRange
{
public:
	int m_min = 0;
	int m_max = 0;

	static const IntRange ZERO;
	static const IntRange ONE;
	static const IntRange ZERO_TO_ONE;

public:
	IntRange() = default;
	explicit IntRange(int min, int max);

	//Accessors
	bool		IsOnRange(int value) const;
	bool		IsOverlappingWith(IntRange const& otherRange) const;

	// -------------------------------------------------------
	void		operator=(IntRange const& copyFrom);
	bool		operator==(IntRange const& compare) const;
	bool		operator!=(IntRange const& compare) const;
}; 
