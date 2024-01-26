#include "AABB3.hpp"
#include "MathUtils.hpp"

AABB3::AABB3(AABB3 const& copyFrom)
	:m_maxs(copyFrom.m_maxs)
	,m_mins(copyFrom.m_mins)
{

}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	:m_mins(Vec3(minX,minY,minZ))
	,m_maxs(Vec3(maxX,maxY,maxZ))
{

}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
	:m_mins(mins)
	,m_maxs(maxs)
{

}

Vec3 const AABB3::GetCenter() const
{
// 	float x = Interpolate(m_mins.x, m_maxs.x, 0.5f);
// 	float y = Interpolate(m_mins.y, m_maxs.y, 0.5f);
// 	float z = Interpolate(m_mins.z, m_maxs.z, 0.5f);
// 	return Vec3(x, y, z);
	return (m_mins + m_maxs) * 0.5f;
}

bool AABB3::IsPointInside(Vec3 const& point) const
{
	if (point.x > m_mins.x && point.x < m_maxs.x && point.y > m_mins.y && point.y < m_maxs.y && point.z > m_mins.z && point.z < m_maxs.z)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vec3 const AABB3::GetNearestPoint(Vec3 const& referencePosition) const
{
	float nearestX = GetClamped(referencePosition.x, m_mins.x, m_maxs.x);
	float nearestY = GetClamped(referencePosition.y, m_mins.y, m_maxs.y);
	float nearestZ = GetClamped(referencePosition.z, m_mins.z, m_maxs.z);
	return Vec3(nearestX, nearestY, nearestZ);
}

