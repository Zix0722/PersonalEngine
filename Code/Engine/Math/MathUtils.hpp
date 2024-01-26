#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
//
// Forward type declarations
//
struct IntVec2;
struct AABB2;
struct OBB2;
struct Vec4;
struct Mat44;
class Actor;

enum class BillboardType
{
	NONE = -1,
	WORLD_UP_CAMERA_FACING,
	WORLD_UP_CAMERA_OPPOSING,
	FULL_CAMERA_FACING,
	FULL_CAMERA_OPPOSING,
	COUNT
};

//
// Angle utilities 
//
float		ConvertDegreesToRadians(float degrees);
float		ConvertRadiansToDegrees(float radians);
float		CosDegrees(float degrees);
float		SinDegrees(float degrees);
float		Atan2Degrees(float y, float x);
float		GetShortestAngularDispDegrees(float startDegrees, float endDegrees);
float		GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees);
float		GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b);


//-------------------------------------------------------------
// Basic 2D & 3D utilities
//
float		GetDistance2D(Vec2 const& positionA, Vec2 const& positionB);
float		GetDistanceSquared2D(Vec2 const& postionA, Vec2 const& positionB);
float		GetDistance3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB);
float		GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB);
int			GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float		GetProjectedLength2D(Vec2 const& vetorToProject, Vec2 const& vectorToProjectOnto);
float		GetProjectedLength3D(Vec3 const& vetorToProject, Vec3 const& vectorToProjectOnto);
Vec2 const	GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto);
Vec3 const  GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);
Mat44		GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale = Vec2(1.f, 1.f));

//------------------------------------------------------------------------------
// Geometric query utilities
//
bool		IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool		IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float radius);
bool		IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box);
bool		IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& start, Vec2 const& end, float radius);
bool		IsPointInsideCapsule3D(Vec3 const& point, Vec3 const& start, Vec3 const& end, float radius);
bool		IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox);
bool		IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool		IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool		DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool		DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool		PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint);
bool		PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool		PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius);
bool		PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox);
Vec2 const	GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius);
Vec2 const	GetNearestPointOnAABB2D(Vec2 const& referencePosition, AABB2 const& box);
Vec2 const	GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, Vec2 const& pointOnLine, Vec2 const& otherPointOnLine);
Vec2 const	GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, Vec2 const& lineSegStart, Vec2 const& lineSegEnd);
Vec3 const  GetNearestPointOnLineSegment3D(Vec3 const& referencePosition, Vec3 const& lineSegStart, Vec3 const& lineSegEnd);
Vec2 const	GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 const	GetNearestPointOnOBB2D(Vec2 const& referencePosition, OBB2 const& orientedBox);

//
// Transform utilities
//
void		TransformPosition2D(Vec2& posTotransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void		TransformPosition2D(Vec2& posTotransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void		TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float zRotationDegrees, Vec2 const& translationXY);
void		TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY);
void		TransformPosition3D(Vec3& positionToTransform, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& translation);

//------------------------------------------------------------------------------
// Clamp  and lerp
float		GetClamped(float value, float minValue, float maxValue);
float		GetClampedZeroToOne(float value);
float		Interpolate(float start, float end, float fractionTowardEnd);
float		GetFractionWithinRange(float value, float rangeStart, float rangeEnd);
float		RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float		RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd);
int			RoundDownToInt(float value);


//------------------------------------------------------------------------------
// Dot and Cross
float		DotProduct2D(Vec2 const& a, Vec2 const& b);
float		DotProduct3D(Vec3 const& a, Vec3 const& b);
float		DotProduct4D(Vec4 const& a, Vec4 const& b);
float		CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3		CrossProduct3D(Vec3 const& a, Vec3 const& b);

//------------------------------------------------------------------------------
//NormalizeByte & DenormalizeByte
float			NormalizeByte(unsigned char byteValue);
unsigned char	DenormalizeByte(float zeroToOne);
//------------------------------------------------------------------------------
//1D Bezier standalone functions
float			ComputeCubicBezier1D(float A, float B, float C, float D, float t);
float			ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t);

//-------------------------------------------------------------------------------------
//Raycast Result
struct RaycastResult2D
{
public:
	// Basic ray cast result information (required)
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec2	m_impactPos;
	Vec2	m_impactNormal;

	// Original ray cast information (optional)
	//Vec2	m_rayFwdNormal;
	//Vec2	m_rayStartPos;
	//float	m_rayMaxLength = 1.f;
	
public:
	RaycastResult2D() {};
	~RaycastResult2D() {};
	RaycastResult2D(const RaycastResult2D& copyFrom);
	explicit RaycastResult2D(bool didImpact, float impactDist, Vec2 impactPos, Vec2 impactNormal);
};

struct RaycastResult3D
{
public:
	bool m_didImpact = false;
	float m_impactDistance = 0.f;
	Vec3 m_impactPosition;
	Vec3 m_impactNormal;
	Vec3 m_rayStartPosition;
	Vec3 m_rayDirection;
	float m_rayLength = 1.f;
	Actor* m_touchedActor = nullptr;
};

RaycastResult2D RaycastVsDisc2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& discCenter, float discRadius);
RaycastResult2D RaycastVsLineSegment2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& lineStart, Vec2 const& lineEnd);
RaycastResult2D RaycastVsAABB2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& AABBMin, Vec2 const& AABBMax);
RaycastResult3D RaycastVsCylinderZ3D(Vec3 const& start, Vec3 const& direction, float distance, Vec2 const& center, float minZ, float maxZ, float radius);
