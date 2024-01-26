#pragma once


struct IntVec2
{
public:
	int x = 0;
	int y = 0;

public:
	//construction /destruction
	~IntVec2() {};
	IntVec2() {};
	IntVec2(const IntVec2& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	//Accessors (const)
	float			GetLength() const;
	int				GetTaxicabLength() const;
	int				GetLengthSquared() const;
	float			GetOrientationRadians() const;
	float			GetOrientationDegrees() const;
	IntVec2	const	GetRotated90Degrees() const;
	IntVec2	const	GetRotatedMinus90Degrees() const;

	//Mutators (non- const)
	
	void SetFromText(const char* text);
	// Operators (const)
	bool		operator==(const IntVec2& compare) const;		// vec2 == vec2
	bool		operator!=(const IntVec2& compare) const;		// vec2 != vec2
	bool		operator<(const IntVec2& comparewith) const;
	const IntVec2	operator+(const IntVec2& vecToAdd) const;		// vec2 + vec2
	const IntVec2	operator-(const IntVec2& vecToSubtract) const;	// vec2 - vec2
	const IntVec2	operator-() const;								// -vec2, i.e. "unary negation"
	const IntVec2	operator*(int uniformScale) const;			// vec2 * float
	const IntVec2	operator*(const IntVec2& vecToMultiply) const;	// vec2 * vec2
	const IntVec2	operator/(int inverseScale) const;			// vec2 / float

	// Operators (self-mutating / non-const)
	void		operator+=(const IntVec2& vecToAdd);				// vec2 += vec2
	void		operator-=(const IntVec2& vecToSubtract);		// vec2 -= vec2
	void		operator*=(const int uniformScale);			// vec2 *= float
	void		operator/=(const int uniformDivisor);		// vec2 /= float
	void		operator=(const IntVec2& copyFrom);				// vec2 = vec2


	void		Rotate90Degrees();
	void		RotateMinus90Degrees();

};