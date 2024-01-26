#define _USE_MATH_DEFINES
#include <math.h>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "IntVec2.hpp"
#include "AABB2.hpp"
#include "OBB2.hpp"
#include "MathUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"
#include "Vec4.hpp"
#include "Engine/Math/Mat44.hpp"


float ConvertDegreesToRadians(float degrees)
{
	return degrees / (180.f / float(M_PI));
}

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / float(M_PI));
}

float CosDegrees(float degrees)
{
	return cosf(ConvertDegreesToRadians(degrees));
}

float SinDegrees(float degrees)
{
	return sinf(ConvertDegreesToRadians(degrees));
}

float Atan2Degrees(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

float GetShortestAngularDispDegrees(float startDegrees, float endDegrees)
{
	float anagleDisp = endDegrees - startDegrees;
	while (anagleDisp > 180.f)
	{
		anagleDisp -= 360.f;
	}
	while (anagleDisp < -180.f)
	{
		anagleDisp += 360.f;
	}
	return anagleDisp;
}

float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees)
{
	float angDisp = GetShortestAngularDispDegrees(currentDegrees, goalDegrees);
	if (fabsf(angDisp) <= maxDeltaDegrees)
	{
		return goalDegrees;
	}
	if (angDisp > 0.f)
	{
		return currentDegrees + maxDeltaDegrees;
	}
	else
	{
		return currentDegrees - maxDeltaDegrees;
	}
	
}

float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b)
{
	float ADotB = DotProduct2D(a, b);
	float angleResult = ConvertRadiansToDegrees(acosf(ADotB / (a.GetLength() * b.GetLength())));

	return angleResult;
}

float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;

	return sqrtf(xDiff * xDiff + yDiff * yDiff);
}

float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;

	return xDiff * xDiff + yDiff * yDiff;
}

float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;
	float zDiff = positionA.z - positionB.z;
	return sqrtf(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
}

float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;
	float zDiff = positionA.z - positionB.z;
	return xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
}

float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;

	return sqrtf(xDiff * xDiff + yDiff * yDiff);
}

float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float xDiff = positionA.x - positionB.x;
	float yDiff = positionA.y - positionB.y;

	return xDiff * xDiff + yDiff * yDiff;
}

int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	IntVec2 displacementVec = pointA - pointB;
	return displacementVec.GetTaxicabLength();
}

float GetProjectedLength2D(Vec2 const& vetorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 normalizedOntoVector = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct2D(vetorToProject, normalizedOntoVector);/*vetorToProject.x * normalizedOntoVector.x + vetorToProject.y * normalizedOntoVector.y;*/
	return projectedLength;
}

float GetProjectedLength3D(Vec3 const& vetorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 normalizedOntoVector = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct3D(vetorToProject, normalizedOntoVector);
	return projectedLength;
}

Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 projectedVector = vectorToProjectOnto.GetNormalized();
	projectedVector *= GetProjectedLength2D(vectorToProject, vectorToProjectOnto);
	return projectedVector;
}

Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 projectedVector = vectorToProjectOnto.GetNormalized();
	projectedVector *= GetProjectedLength3D(vectorToProject, vectorToProjectOnto);
	return projectedVector;
}

Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& cameraMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale /*= Vec2(1.f, 1.f)*/)
{
	Vec3 billboardIBasis = Vec3(1.f, 0.f, 0.f);
	Vec3 billboardJBasis = Vec3(0.f, 1.f, 0.f);
	Vec3 billboardKBasis = Vec3(0.f, 0.f, 1.f);
	Vec3 billboardTranslation = billboardPosition;

	Vec3 cameraIBasis = cameraMatrix.GetIBasis3D();
	Vec3 cameraJBasis = cameraMatrix.GetJBasis3D();
	Vec3 cameraKBasis = cameraMatrix.GetKBasis3D();
	Vec3 cameraPosition = cameraMatrix.GetTranslation3D();
	
	Vec3 Zaxis = Vec3(0.f, 0.f, 1.f);
	Vec3 Yaxis = Vec3(0.f, 1.f, 0.f);

	switch (billboardType)
	{
	case BillboardType::NONE:
		break;
	case BillboardType::WORLD_UP_CAMERA_FACING:
		billboardIBasis = -cameraIBasis;
		billboardJBasis = -cameraJBasis;
		billboardKBasis = Zaxis;
		break;
	case BillboardType::WORLD_UP_CAMERA_OPPOSING:
		billboardKBasis = Zaxis;
		billboardIBasis = -cameraIBasis;
		billboardIBasis.z = 0.f;
		billboardIBasis = billboardIBasis.GetNormalized();
// 		billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis); 
// 		billboardJBasis = billboardJBasis.GetNormalized();
		billboardJBasis = -cameraJBasis;
		break;
	case BillboardType::FULL_CAMERA_FACING:
		billboardIBasis = billboardTranslation - cameraPosition;
		billboardIBasis = billboardIBasis.GetNormalized();
		if (abs(DotProduct3D(billboardIBasis, Zaxis) < 1.f))
		{
			billboardJBasis = CrossProduct3D(Zaxis, billboardIBasis);
			billboardJBasis = billboardJBasis.GetNormalized();
			billboardKBasis = CrossProduct3D(billboardIBasis, billboardJBasis);
			billboardKBasis = billboardKBasis.GetNormalized();
		}
		else
		{
			billboardKBasis = CrossProduct3D(billboardIBasis, Yaxis);
			billboardKBasis = billboardKBasis.GetNormalized();
			billboardJBasis = CrossProduct3D(billboardKBasis, billboardIBasis);
			billboardJBasis = billboardJBasis.GetNormalized();
		}
		break;
	case BillboardType::FULL_CAMERA_OPPOSING:
		billboardIBasis = -cameraIBasis;
		billboardJBasis = -cameraJBasis;
		billboardKBasis = cameraKBasis;
		break;
	case BillboardType::COUNT:
		break;
	default:
		break;
	}
	Mat44 result = Mat44(billboardIBasis, billboardJBasis, billboardKBasis, billboardTranslation);
	result.AppendScaleNonUniform2D(billboardScale);
	return result;
}

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	return GetDistanceSquared2D(discCenter, point) < discRadius * discRadius;
}

bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float radius)
{
	return GetDistanceSquared3D(sphereCenter, point) < radius * radius;
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	return box.IsPointInside(point);
}

bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& start, Vec2 const& end, float radius)
{
	if (IsPointInsideDisc2D(point, start, radius))
	{
		return true;
	}
	if (IsPointInsideDisc2D(point, end, radius))
	{
		return true;
	}

	Vec2 nearestPointOnCapsuleBone = GetNearestPointOnLineSegment2D(point, start, end);
	float distanceToPoint = GetDistance2D(point, nearestPointOnCapsuleBone);
	if (distanceToPoint < radius)
	{
		return true;
	}

	return false;
}

bool IsPointInsideCapsule3D(Vec3 const& point, Vec3 const& start, Vec3 const& end, float radius)
{
	if (IsPointInsideSphere3D(point, start, radius))
	{
		return true;
	}
	if (IsPointInsideSphere3D(point, end, radius))
	{
		return true;
	}

	Vec3 nearestPointOnCapsuleBone = GetNearestPointOnLineSegment3D(point, start, end);
	float distanceToPoint = GetDistance3D(point, nearestPointOnCapsuleBone);
	if (distanceToPoint < radius)
	{
		return true;
	}

	return false;
}

bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox)
{
	Vec2 pointCenterDisp = point - orientedBox.m_center;
	float lengthOnJBasis = GetProjectedLength2D(pointCenterDisp, orientedBox.GetJBasisNormal());
	float lengthOnIBasis = GetProjectedLength2D(pointCenterDisp, orientedBox.m_iBasisNormal);
	if (fabsf(lengthOnIBasis) >= orientedBox.m_halfDimensions.x)
	{
		return false;
	}
	if (fabsf(lengthOnJBasis) >= orientedBox.m_halfDimensions.y)
	{
		return false;
	}
	return true;
}

bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	if (GetDistanceSquared2D(point, sectorTip) >= sectorRadius * sectorRadius)
	{
		return false;
	}
	Vec2 tangant(1.f, 0.f);
	Vec2 sectorForwardNormal = tangant.GetRotatedDegrees(sectorForwardDegrees);

	return IsPointInsideDirectedSector2D(point, sectorTip, sectorForwardNormal, sectorApertureDegrees, sectorRadius);
}

bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	if (GetDistanceSquared2D(point, sectorTip) >= sectorRadius * sectorRadius)
	{
		return false;
	}
	Vec2 displacementVector =  point - sectorTip;
	float degreesOffCenter = GetAngleDegreesBetweenVectors2D(displacementVector, sectorForwardNormal);
	if (degreesOffCenter >= sectorApertureDegrees * 0.5f)
	{
		return false;
	}
	return true;
}

bool DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	float ABDistance = GetDistance2D(centerA, centerB);
	float RadiusSum = radiusA + radiusB;
	if (ABDistance < RadiusSum)
	{
		return true;
	}
	return false;
}

bool DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float ABDistance = GetDistance3D(centerA, centerB);
	float RadiusSum = radiusA + radiusB;
	if (ABDistance < RadiusSum)
	{
		return true;
	}
	return false;
}

Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius)
{
	Vec2 nearestPoint = Vec2(referencePosition.x - discCenter.x, referencePosition.y - discCenter.y);
	nearestPoint.ClampLength(discRadius);
	nearestPoint += discCenter;
	return nearestPoint;
}

Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePosition, AABB2 const& box)
{
	float nearestX = GetClamped(referencePosition.x, box.m_mins.x, box.m_maxs.x);
	float nearestY = GetClamped(referencePosition.y, box.m_mins.y, box.m_maxs.y);

	return Vec2(nearestX, nearestY);
}

Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, Vec2 const& pointOnLine, Vec2 const& otherPointOnLine)
{
	Vec2 startEndDisp = otherPointOnLine - pointOnLine;
	Vec2 pointStartDisp = referencePosition - pointOnLine;

	Vec2 dispAlongLineSeg = GetProjectedOnto2D(pointStartDisp, startEndDisp);
	return pointOnLine + dispAlongLineSeg;
}

Vec2 const GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, Vec2 const& lineSegStart, Vec2 const& lineSegEnd)
{
	Vec2 startEndDisp = lineSegEnd - lineSegStart;
	Vec2 pointStartDisp = referencePosition - lineSegStart;
	Vec2 pointEndDisp = referencePosition - lineSegEnd;

	if (DotProduct2D(startEndDisp, pointStartDisp) < 0.f)
	{
		return lineSegStart;
	}

	if (DotProduct2D(startEndDisp, pointEndDisp) > 0.f)
	{
		return lineSegEnd;
	}

	Vec2 dispAlongLineSeg = GetProjectedOnto2D(pointStartDisp, startEndDisp);
	return lineSegStart + dispAlongLineSeg;
}



Vec3 const GetNearestPointOnLineSegment3D(Vec3 const& referencePosition, Vec3 const& lineSegStart, Vec3 const& lineSegEnd)
{
	Vec3 startEndDisp = lineSegEnd - lineSegStart;
	Vec3 pointStartDisp = referencePosition - lineSegStart;
	Vec3 pointEndDisp = referencePosition - lineSegEnd;

	if (DotProduct3D(startEndDisp, pointStartDisp) < 0.f)
	{
		return lineSegStart;
	}

	if (DotProduct3D(startEndDisp, pointEndDisp) > 0.f)
	{
		return lineSegEnd;
	}

	Vec3 dispAlongLineSeg = GetProjectedOnto3D(pointStartDisp, startEndDisp);
	return lineSegStart + dispAlongLineSeg;
}

Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 nearestPointOnBone = GetNearestPointOnLineSegment2D(referencePosition, boneStart, boneEnd);
	Vec2 nearestPointReferenceDisp = referencePosition - nearestPointOnBone;
	nearestPointReferenceDisp.ClampLength(radius);

	return nearestPointOnBone + nearestPointReferenceDisp;
}

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint)
{
	if (GetDistanceSquared2D(fixedPoint, mobileDiscCenter) >= discRadius * discRadius)
	{
		return false;
	}
	Vec2 displacementVector = mobileDiscCenter - fixedPoint;
	displacementVector.SetLength(discRadius - displacementVector.GetLength());
	mobileDiscCenter += displacementVector;
	return true;

}

bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	if (GetDistanceSquared2D(fixedDiscCenter, mobileDiscCenter) >= (mobileDiscRadius + fixedDiscRadius) * (mobileDiscRadius + fixedDiscRadius))
	{
		return false;
	}
	Vec2 displacementVector = mobileDiscCenter - fixedDiscCenter;
	displacementVector.SetLength((mobileDiscRadius + fixedDiscRadius) - displacementVector.GetLength());
	mobileDiscCenter += displacementVector;
	return true;
}

bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius)
{
	if (GetDistanceSquared2D(aCenter, bCenter) >= (aRadius + bRadius) * (aRadius + bRadius))
	{
		return false;
	}
	Vec2 displacementVector = aCenter - bCenter;
	displacementVector.SetLength(((aRadius + bRadius) - displacementVector.GetLength()) * 0.5f);
	aCenter += displacementVector;
	bCenter -= displacementVector;
	return true;
}

bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox)
{
	Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint);
}

Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePosition, OBB2 const& orientedBox)
{
	if (IsPointInsideOBB2D(referencePosition, orientedBox))
	{
		return referencePosition;
	}
	Vec2 centerReferenceDisp = referencePosition - orientedBox.m_center;

	float lentghOnIBasis = GetProjectedLength2D(centerReferenceDisp, orientedBox.m_iBasisNormal);
	float lengthOnJBasis = GetProjectedLength2D(centerReferenceDisp, orientedBox.GetJBasisNormal());

	lentghOnIBasis = GetClamped(lentghOnIBasis, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	lengthOnJBasis = GetClamped(lengthOnJBasis, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);

	Vec2 nearestPointInWorldCoord = orientedBox.m_center + (lentghOnIBasis * orientedBox.m_iBasisNormal) + (lengthOnJBasis * orientedBox.GetJBasisNormal());
	return nearestPointInWorldCoord;
}

void TransformPosition2D(Vec2& posTotransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	posTotransform.x *= uniformScale;
	posTotransform.y *= uniformScale;

	posTotransform.RotateDegrees(rotationDegrees);
	posTotransform.x += translation.x;
	posTotransform.y += translation.y;
}

void TransformPosition2D(Vec2& posTotransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	float newX = posTotransform.x * iBasis.x + posTotransform.y * jBasis.x;
	float newY = posTotransform.x * iBasis.y + posTotransform.y * jBasis.y;
	Vec2 transformedVec(newX, newY);
	transformedVec += translation;
	posTotransform = transformedVec;
}

void TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float zRotationDegrees, Vec2 const& translationXY)
{
	Vec3 result = positionToTransform.GetRotatedAboutZDegrees(zRotationDegrees);
	positionToTransform.x = result.x;
	positionToTransform.y = result.y;
	positionToTransform.x *= scaleXY;
	positionToTransform.y *= scaleXY;

	positionToTransform.x += translationXY.x;
	positionToTransform.y += translationXY.y;
}

void TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY)
{
	float newX = positionToTransform.x * iBasis.x + positionToTransform.y * jBasis.x;
	float newY = positionToTransform.x * iBasis.y + positionToTransform.y * jBasis.y;
	Vec2 transformedVec(newX, newY);
	transformedVec += translationXY;
	positionToTransform = Vec3(transformedVec.x, transformedVec.y, positionToTransform.z);
}
// wrong 
void TransformPosition3D(Vec3& positionToTransform, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& translation)
{
	float newX = positionToTransform.x * iBasis.x + positionToTransform.y * jBasis.x + positionToTransform.z * kBasis.x;
	float newY = positionToTransform.x * iBasis.y + positionToTransform.y * jBasis.y + positionToTransform.z * kBasis.y;
	float newZ = positionToTransform.x * iBasis.z + positionToTransform.y * jBasis.z + positionToTransform.z * kBasis.z;
	Vec3 transformedVec(newX, newY, newZ);
	transformedVec += translation;
	positionToTransform = Vec3(transformedVec.x, transformedVec.y, transformedVec.z);
}

float GetClamped(float value, float minValue, float maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	else if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}

float GetClampedZeroToOne(float value)
{
	return GetClamped(value, 0.f,1.f);
}

float Interpolate(float start, float end, float fractionTowardEnd)
{
	float displacement = end - start;
	float result = displacement * fractionTowardEnd;
	result += start;
	return result;
}

float GetFractionWithinRange(float value, float rangeStart, float rangeEnd)
{
	float range = rangeEnd - rangeStart;
	float valFromRange = value - rangeStart;

	return valFromRange / range;
}

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float fraction = GetFractionWithinRange(inValue, inStart, inEnd);
	float outRange = outEnd - outStart;
	float result = outRange * fraction;
	result += outStart; 
	return result;
}

float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	if (inValue < inStart)
	{
		return outStart;
	}
	if (inValue > inEnd)
	{
		return outEnd;
	}
	// do same as rangeMap
	float fraction = GetFractionWithinRange(inValue, inStart, inEnd);
	float outRange = outEnd - outStart;
	float result = outRange * fraction;
	result += outStart;
	return result;

}

int RoundDownToInt(float value)
{
	int result = static_cast<int>(floorf(value));
	return result;
}

float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return a.x * b.x + a.y * b.y;
}

float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	return a.x * b.y - a.y * b.x;
}

Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	return Vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
}

float NormalizeByte(unsigned char byteValue)
{
	if (byteValue == 0)
	{
		return 0.f;
	}
	return RangeMap(static_cast<float>(byteValue), 0.f, 255.f, 0.f, 1.f);
}

unsigned char DenormalizeByte(float zeroToOne)
{
	if (zeroToOne == 0.f)
	{
		return static_cast<unsigned char>(0);
	}
	if (zeroToOne == 1.f)
	{
		return static_cast<unsigned char>(255);
	}
	return static_cast<unsigned char>(zeroToOne * 256 );
}

float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	float s = 1.f - t;
	float midAB, midBC, midCD, midABBC, midBCCD, midFinal;

	midAB = s * A + t * B;
	midBC = s * B + t * C;
	midCD = s * C + t * D;

	midABBC = s * midAB + t * midBC;
	midBCCD = s * midBC + t * midCD;

	midFinal = s * midABBC + t * midBCCD;
	return midFinal;
}

float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	float s = 1.f - t;
	float midAB, midBC, midCD, midDE, midEF, midABBC, midBCCD, midCDDE, midDEEF;
	float midABBCBCCD, midBCCDCDDE, midCDDEDEEF;
	float midAToE, midBToF;
	float midFinal;

	midAB = s * A + t * B;
	midBC = s * B + t * C;
	midCD = s * C + t * D;
	midDE = s * D + t * E;
	midEF = s * E + t * F;

	midABBC = s * midAB + t * midBC;
	midBCCD = s * midBC + t * midCD;
	midCDDE = s * midCD + t * midDE;
	midDEEF = s * midDE + t * midEF;

	midABBCBCCD = s * midABBC + t * midBCCD;
	midBCCDCDDE = s * midBCCD + t * midCDDE;
	midCDDEDEEF = s * midCDDE + t * midDEEF;

	midAToE = s * midABBCBCCD + t * midBCCDCDDE;
	midBToF = s * midBCCDCDDE + t * midCDDEDEEF;

	midFinal = s * midAToE + t * midBToF;
	return midFinal;
}

RaycastResult2D::RaycastResult2D(const RaycastResult2D& copyFrom)
	:m_didImpact(copyFrom.m_didImpact)
	, m_impactDist(copyFrom.m_impactDist)
	, m_impactNormal(copyFrom.m_impactNormal)
	, m_impactPos(copyFrom.m_impactPos)
{

}

RaycastResult2D::RaycastResult2D(bool didImpact, float impactDist, Vec2 impactPos, Vec2 impactNormal)
	:m_didImpact(didImpact)
	, m_impactDist(impactDist)
	, m_impactPos(impactPos)
	, m_impactNormal(impactNormal)
{

}

RaycastResult2D RaycastVsDisc2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& discCenter, float discRadius)
{
	Vec2 jNoramal = Vec2(-fwdNormal.y, fwdNormal.x);
	Vec2 scDisp = discCenter - startPos;
	float scJ = DotProduct2D(scDisp, jNoramal);
	if (scJ > discRadius || scJ < -discRadius)
	{
		return RaycastResult2D(false, 0.f, Vec2::ZERO, Vec2::ZERO);
	}
	else
	{
		float scI = DotProduct2D(scDisp, fwdNormal);
		if (scI < -discRadius || scI > maxDist + discRadius)
		{
			if (IsPointInsideDisc2D(startPos, discCenter, discRadius))
			{
				bool didImpact = true;
				float impactDist = 0.f;
				Vec2 impactPos = startPos;
				Vec2 impactNormal = -fwdNormal;
				return RaycastResult2D(didImpact, impactDist, impactPos, impactNormal);
			}
		}
		float distanceInsideDisc = sqrtf(discRadius * discRadius - scJ * scJ);
		float impactDist = scI - distanceInsideDisc;
		if (impactDist < 0.f || impactDist > maxDist)
		{
			return RaycastResult2D(false, 0.f, Vec2::ZERO, Vec2::ZERO);
		}
		else
		{
			Vec2 impactPos = startPos + fwdNormal * impactDist;
			Vec2 impactNormal = (impactPos - discCenter).GetNormalized();
			return RaycastResult2D(true, impactDist, impactPos, impactNormal);
		}

	}
}

RaycastResult2D RaycastVsLineSegment2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& lineStart, Vec2 const& lineEnd)
{
	RaycastResult2D result;
	Vec2 jLeft = fwdNormal.GetRotated90Degrees();
	Vec2 dispRaySLineS = lineStart - startPos;
	Vec2 dispRaySLineE = lineEnd - startPos;
	float dispSSonJ = DotProduct2D(dispRaySLineS, jLeft);
	float dispSEonJ = DotProduct2D(dispRaySLineE, jLeft);
	float t = -dispSSonJ / (dispSEonJ - dispSSonJ);
	if (!(t > 0.f && t < 1.f))
	{
		return result;
	}
	
	Vec2 impactPos = lineStart + t * (lineEnd - lineStart);
	Vec2 dispRaySImpactPos = impactPos - startPos;
	float impactDist = DotProduct2D(dispRaySImpactPos, fwdNormal);
	if (impactDist >= maxDist || impactDist < 0.f)
	{
		return result;
	}
	Vec2 dispLineSLineE = lineEnd - lineStart;
	Vec2 lineIFwd = dispLineSLineE.GetNormalized();
	Vec2 resultNormal = lineIFwd.GetRotated90Degrees();
	if (DotProduct2D(fwdNormal, resultNormal) > 0)
	{
		resultNormal *= -1;
	}
	if (dispSSonJ * dispSEonJ >= 0.f)
	{
		return result;
	}
	result.m_didImpact = true;
	result.m_impactDist = impactDist;
	result.m_impactNormal = resultNormal;
	result.m_impactPos = impactPos;
	return result;
}

RaycastResult2D RaycastVsAABB2D(Vec2 const& startPos, Vec2 const& fwdNormal, float maxDist, Vec2 const& AABBMin, Vec2 const& AABBMax)
{
	RaycastResult2D result;
	if (startPos.x > AABBMin.x && startPos.x < AABBMax.x && startPos.y > AABBMin.y && startPos.y < AABBMax.y)
	{
		result.m_didImpact = true;
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}
	
	Vec2 endPos = startPos + fwdNormal * maxDist;
	Vec2 disp = endPos - startPos;
	float t = 999.f;
	int mark = -1;
	float tXmin = (AABBMin.x - startPos.x) / disp.x;
	float tYmin = (AABBMin.y - startPos.y) / disp.y;
	float tXmax = (AABBMax.x - startPos.x) / disp.x;
	float tYmax = (AABBMax.y - startPos.y) / disp.y;

	if (tXmin < t)
	{
		if (tXmin > 0.f && tXmin < 1.f)
		{
			float py = startPos.y + tXmin * (disp.y);
			if (py > AABBMin.y && py < AABBMax.y)
			{
				mark = 1;
				t = tXmin;
			}
		}
	}
	if (tYmin < t)
	{
		if (tYmin > 0.f && tYmin < 1.f)
		{
			float px = startPos.x + tYmin * (disp.x);
			if (px > AABBMin.x && px < AABBMax.x)
			{
				mark = 2;
				t = tYmin;
			}
		}
	}
	if (tXmax < t)
	{
		if (tXmax > 0.f && tXmax < 1.f)
		{
			float py = startPos.y + tXmax * (disp.y);
			if (py > AABBMin.y && py < AABBMax.y)
			{
				mark = 3;
				t = tXmax;
			}
		}
	}
	if (tYmax < t)
	{
		if (tYmax > 0.f && tYmax < 1.f)
		{
			
			float px = startPos.x + tYmax * (disp.x);
			if (px > AABBMin.x && px < AABBMax.x)
			{
				mark = 4;
				t = tYmax;
			}
		}
	}
	Vec2 impactPos;
	Vec2 impactNormal;
	switch (mark)
	{
	case 1:
		impactPos = startPos + tXmin * (endPos - startPos);
		impactNormal = Vec2(-1.f, 0.f);
		break;
	case 2:
		impactPos = startPos + tYmin * (endPos - startPos);
		impactNormal = Vec2(0.f, -1.f);
		break;
	case 3:
		impactPos = startPos + tXmax * (endPos - startPos);
		impactNormal = Vec2(1.f, 0.f);
		break;
	case 4:
		impactPos = startPos + tYmax * (endPos - startPos);
		impactNormal = Vec2(0.f, 1.f);
		break;
	default:
		
		return result;
	}

	result.m_didImpact = true;
	result.m_impactDist = (impactPos - startPos).GetLength();
	result.m_impactPos = impactPos;
	result.m_impactNormal = impactNormal;
	return result;
	
}

RaycastResult3D RaycastVsCylinderZ3D(Vec3 const& start, Vec3 const& direction, float distance, Vec2 const& center, float minZ, float maxZ, float radius)
{
	RaycastResult3D result;
	result.m_didImpact = false;
	result.m_rayStartPosition = start;
	result.m_rayDirection = direction;
	result.m_rayLength = distance;
	result.m_impactDistance = distance;
	
	if (IsPointInsideDisc2D(Vec2(start), center, radius) && start.z < maxZ && start.z > minZ)
	{
		result.m_didImpact = true;
		result.m_impactDistance = 0.f;
		result.m_impactNormal = -direction;
		result.m_impactPosition = start;
		return result;
	}
	else
	{
		RaycastResult2D result2D = RaycastVsDisc2D(Vec2(start), Vec2(direction).GetNormalized(), GetProjectedLength2D(Vec2(direction * distance), Vec2(direction).GetNormalized()), center, radius);
		if (result2D.m_didImpact == false)
		{
			result.m_didImpact = false;
			result.m_impactPosition = Vec3(0.f,0.f,0.f);
			result.m_impactDistance = 0.f;
			result.m_impactNormal = Vec3(0.f, 0.f, 0.f);
			return result;
		}
		float zComponent = direction.z;
		float distToBott = (minZ - start.z) / zComponent;
		float distToTop = (maxZ - start.z) / zComponent;

		if (distToBott > 0.f && distToTop > 0.f) // higher than top or lower than bottom
		{
			if (distToBott > distToTop) // higher than Top
			{
				if (result2D.m_didImpact)
				{
					Vec3 impactPosition = start + direction * distToTop;
					if (IsPointInsideDisc2D(Vec2(impactPosition), center, radius))
					{
						result.m_didImpact = true;
						result.m_impactPosition = start + direction * distToTop;
						result.m_impactDistance = distToTop;
						result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
						return result;
					}
					else
					{
						float lengthA = GetProjectedLength2D(Vec2(direction * distToTop), Vec2(direction).GetNormalized());
						float factor = result2D.m_impactDist / lengthA;
						float impactDist = distToTop * factor;
						Vec3 impactPos = start + direction * impactDist;
						if (impactPos.z < maxZ && impactPos.z > minZ)
						{
							result.m_didImpact = true;
							result.m_impactDistance = impactDist;
							result.m_impactPosition = impactPos;
							result.m_impactNormal = Vec3(result2D.m_impactNormal.x, result2D.m_impactNormal.y, 0.f);
							return result;
						}
					}
					
				}
			}
			if (distToBott < distToTop) // lower than Bottom
			{
				if (result2D.m_didImpact)
				{
					Vec3 impactPosition = start + direction * distToBott;
					if (IsPointInsideDisc2D(Vec2(impactPosition), center, radius))
					{
						result.m_didImpact = true;
						result.m_impactPosition = start + direction * distToBott;
						result.m_impactDistance = distToBott;
						result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
						return result;
					}
					else
					{
						float lengthA = GetProjectedLength2D(Vec2(direction * distToBott), Vec2(direction).GetNormalized());
						float factor = result2D.m_impactDist / lengthA;
						float impactDist = distToBott * factor;
						Vec3 impactPos = start + direction * impactDist;
						if (impactPos.z < maxZ && impactPos.z > minZ)
						{
							result.m_didImpact = true;
							result.m_impactDistance = impactDist;
							result.m_impactPosition = impactPos;
							result.m_impactNormal = Vec3(result2D.m_impactNormal.x, result2D.m_impactNormal.y, 0.f);
							return result;
						}
					}

				}
			}
		}
		else if (distToBott > 0.f && distToTop < 0.f)  // aim bottom inside minZ & maxZ
		{
			float lengthA = GetProjectedLength2D(Vec2(direction * distToBott), Vec2(direction).GetNormalized());
			float factor = result2D.m_impactDist / lengthA;
			float impactDist = distToBott * factor;
			Vec3 impactPos = start + direction * impactDist;
			if (impactPos.z < maxZ && impactPos.z > minZ)
			{
				result.m_didImpact = true;
				result.m_impactDistance = impactDist;
				result.m_impactPosition = impactPos;
				result.m_impactNormal = Vec3(result2D.m_impactNormal.x, result2D.m_impactNormal.y, 0.f);
				return result;
			}
		}
		else if (distToBott < 0.f && distToTop > 0.f)  // aim top inside minZ & maxZ
		{
			float lengthA = GetProjectedLength2D(Vec2(direction * distToTop), Vec2(direction).GetNormalized());
			float factor = result2D.m_impactDist / lengthA;
			float impactDist = distToTop * factor;
			Vec3 impactPos = start + direction * impactDist;
			if (impactPos.z < maxZ && impactPos.z > minZ)
			{
				result.m_didImpact = true;
				result.m_impactDistance = impactDist;
				result.m_impactPosition = impactPos;
				result.m_impactNormal = Vec3(result2D.m_impactNormal.x, result2D.m_impactNormal.y, 0.f);
				return result;
			}
		}
		else
		{
			float lengthA = GetProjectedLength2D(Vec2(direction * distToTop), Vec2(direction).GetNormalized());
			float factor = result2D.m_impactDist / lengthA;
			float impactDist = distToTop * factor;
			Vec3 impactPos = start + direction * impactDist;
			if (impactPos.z < maxZ && impactPos.z > minZ)
			{
				result.m_didImpact = true;
				result.m_impactDistance = impactDist;
				result.m_impactPosition = impactPos;
				result.m_impactNormal = Vec3(result2D.m_impactNormal.x, result2D.m_impactNormal.y, 0.f);
				return result;
			}
		}
		
		
	}
	return result;
}

