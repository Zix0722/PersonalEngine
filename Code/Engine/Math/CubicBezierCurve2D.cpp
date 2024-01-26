#include "CubicBezierCurve2D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "CubicHermiteCurve.hpp"

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
	:m_startPos(startPos)
	,m_guidePos1(guidePos1)
	,m_guidePos2(guidePos2)
	,m_endPos(endPos)
{
	
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite)
	:m_startPos(fromHermite.m_startPos)
	,m_endPos(fromHermite.m_endPos)
{
	m_guidePos1 = fromHermite.m_startPos + fromHermite.m_startVel;
	m_guidePos2 = fromHermite.m_endPos - fromHermite.m_endVel;
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec2 result;
	result.x = ComputeCubicBezier1D(m_startPos.x, m_guidePos1.x, m_guidePos2.x, m_endPos.x, parametricZeroToOne);
	result.y = ComputeCubicBezier1D(m_startPos.y, m_guidePos1.y, m_guidePos2.y, m_endPos.y, parametricZeroToOne);
	return result;
}

float CubicBezierCurve2D::GetApproximateLength(int numSubdivisions /*= 64*/) const
{
	float delatT = 1.f / numSubdivisions;
	float totalLength = 0.f;
	for (int index = 0; index < numSubdivisions; ++index)
	{
		float t = index * delatT;
		float tNext = (index + 1) * delatT;
		Vec2 firstPos, nextPos;
		firstPos = EvaluateAtParametric(t);
		nextPos = EvaluateAtParametric(tNext);
		float length = GetDistance2D(firstPos, nextPos);
		totalLength += length;
	}

	return totalLength;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdivisions /*= 64*/) const
{
	float fullLength = GetApproximateLength(numSubdivisions);
	if (distanceAlongCurve >= fullLength)
	{
		return m_endPos;
	}
	if (distanceAlongCurve == 0.f)
	{
		return m_startPos;
	}
	float deltaT = 1.f / numSubdivisions;

	float currentLength = 0.f;
	float currentTime = 0.f;
	Vec2 pointOne, pointTwo;
	float lineLength = 0.f;
	for (int lineIndex = 0; lineIndex < numSubdivisions; ++lineIndex)
	{
		pointOne = EvaluateAtParametric(currentTime);
		pointTwo = EvaluateAtParametric(currentTime + deltaT);
		lineLength = GetDistance2D(pointOne, pointTwo);

		if (currentLength + lineLength > distanceAlongCurve)
		{
			break;
		}
		currentLength += lineLength;
		currentTime += deltaT;
	}
	float fraction = 0.f;
	if (currentLength == 0.f)
	{

	}
	else
	{
		fraction = distanceAlongCurve - currentLength;
	}
	
	float ratio = fraction / lineLength;
	float resultX = Interpolate(pointOne.x, pointTwo.x, ratio);
	float resultY = Interpolate(pointOne.y, pointTwo.y, ratio);
	return Vec2(resultX, resultY);
}

