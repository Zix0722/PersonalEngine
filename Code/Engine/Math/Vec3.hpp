#pragma once

struct Vec2;
//-------------------------------------------------
struct Vec3
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

public:
	// Construction/Destruction
	~Vec3() {}												// destructor (do nothing)
	Vec3() {}												// default constructor (do nothing)
	Vec3(const Vec3& copyFrom);							// copy constructor (from another vec3)
	explicit Vec3(float initialX, float initialY, float initialZ);		// explicit constructor (from x, y,z)

	// Accessors (const methods)
	float		GetLength() const;
	float		GetLengthXY() const;
	float		GetLengthSquared() const;
	float		GetLengthXYSquared() const;
	float		GetAngleAboutZRadians() const;
	float		GetAngleAboutZDegrees() const;
	Vec3 const	GetRotatedAboutZRadians( float deltaRadians ) const;
	Vec3 const	GetRotatedAboutZDegrees( float deltaDegrees ) const;
	Vec3 const	GetClamped(float maxLength) const;
	Vec3 const	GetNormalized() const;
	Vec2 const  GetVec2() const;
	void		SetFromText(char const* text);
	//void		SetLength(float newLength);
	static Vec3 const	MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length = 1.f);
	static Vec3 const	MakeFromPolarDegrees(float latitudeRadians, float longitudeRadians, float length = 1.f);

	// Operators (const)
	bool		operator==(const Vec3& compare) const;		// Vec3 == Vec3
	bool		operator!=(const Vec3& compare) const;		// Vec3 != Vec3
	const Vec3	operator+(const Vec3& vecToAdd) const;		// Vec3 + Vec3
	const Vec3	operator-(const Vec3& vecToSubtract) const;	// Vec3 - Vec3
	const Vec3	operator-() const;								// -Vec3, i.e. "unary negation"
	const Vec3	operator*(float uniformScale) const;			// Vec3 * float
	const Vec3	operator*(const Vec3& vecToMultiply) const;	// Vec3 * Vec3
	const Vec3	operator/(float inverseScale) const;			// Vec3 / float

	// Operators (self-mutating / non-const)
	void		operator+=(const Vec3& vecToAdd);				// Vec3 += Vec3
	void		operator-=(const Vec3& vecToSubtract);		// Vec3 -= Vec3
	void		operator*=(const float uniformScale);			// Vec3 *= float
	void		operator/=(const float uniformDivisor);		// Vec3 /= float
	void		operator=(const Vec3& copyFrom);				// vec3 = vec3

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec3::
	friend const Vec3 operator*(float uniformScale, const Vec3& vecToScale);	// float * Vec3
};

Vec3 const MakeFromPolarDegrees(float orientationDegrees, float length = 1.f);