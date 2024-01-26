#include "Engine/Math/OBB2.hpp"

OBB2::OBB2(const OBB2& copyFrom)
	:m_center(copyFrom.m_center)
	,m_halfDimensions(copyFrom.m_halfDimensions)
	,m_iBasisNormal(copyFrom.m_iBasisNormal)
{

}

OBB2::OBB2(Vec2 center, Vec2 iBasisNormal, Vec2 halfDimensions)
	:m_center(center)
	,m_halfDimensions(halfDimensions)
	,m_iBasisNormal(iBasisNormal)
{

}

void OBB2::GetCornerPoints(Vec2* out_fourCornerWorldPositions) const
{
	Vec2 xBasisDisplacement = m_iBasisNormal * m_halfDimensions.x;
	Vec2 yBasisDisplacement = GetJBasisNormal() * m_halfDimensions.y;

	Vec2 TR = m_center + xBasisDisplacement + yBasisDisplacement;
	Vec2 TL = m_center - xBasisDisplacement + yBasisDisplacement;
	Vec2 BL = m_center - xBasisDisplacement - yBasisDisplacement;
	Vec2 BR = m_center + xBasisDisplacement - yBasisDisplacement;

	out_fourCornerWorldPositions[0] = TR;
	out_fourCornerWorldPositions[1] = TL;
	out_fourCornerWorldPositions[2] = BL;
	out_fourCornerWorldPositions[3] = BR;
}

Vec2 OBB2::GetLocalPosForWorldPos(Vec2 worldPos) const
{
	return Vec2::ZERO;
}

Vec2 OBB2::GetWorldPosForLocalPos(Vec2 localPos) const
{
	return Vec2::ZERO;
}

Vec2 const OBB2::GetJBasisNormal() const
{
	return m_iBasisNormal.GetRotated90Degrees();
}

void OBB2::RotateAboutCenter(float rotationDeltaDegrees)
{
	m_iBasisNormal.RotateDegrees(rotationDeltaDegrees);
}

