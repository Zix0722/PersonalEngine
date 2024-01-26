#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "AABB2.hpp"


const AABB2 AABB2::ZERO_TO_ONE = AABB2(Vec2(0.f,0.f), Vec2(1.f,1.f));

AABB2::AABB2(AABB2 const& copyForm)
{
	m_mins = copyForm.m_mins;
	m_maxs = copyForm.m_maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_maxs.x = maxX;
	m_maxs.y = maxY;
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

bool AABB2::IsPointInside(Vec2 const& point) const
{
	if (point.x > m_mins.x && point.x < m_maxs.x && point.y > m_mins.y && point.y < m_maxs.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vec2 const AABB2::GetCenter() const
{
	return GetPointAtUV(Vec2(0.5f, 0.5f));
}

Vec2 const AABB2::GetDimensions() const
{
	return m_maxs - m_mins;
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& referencePosition) const
{
	float nearestX = GetClamped(referencePosition.x, m_mins.x,m_maxs.x);
	float nearestY = GetClamped(referencePosition.y, m_mins.y, m_maxs.y);
	return Vec2(nearestX, nearestY);
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float PointX = RangeMap(uv.x, 0.f, 1.f, m_mins.x, m_maxs.x);
	float PointY = RangeMap(uv.y, 0.f, 1.f, m_mins.y, m_maxs.y);

	return Vec2(PointX, PointY);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	float u = RangeMap(point.x, m_mins.x, m_maxs.x, 0.f, 1.f);
	float v = RangeMap(point.y, m_mins.y, m_maxs.y, 0.f, 1.f);
	return Vec2(u, v);
}

void AABB2::Translate(Vec2 const& translationToApply)
{
	m_mins += translationToApply;
	m_maxs += translationToApply;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 move = newCenter - GetCenter();
	Translate(move);
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2 newCenter = GetCenter();
	m_mins = Vec2(0.f, 0.f);
	m_maxs = newDimensions;
	SetCenter(newCenter);
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (!IsPointInside(point))
	{
		if (point.x < m_mins.x)
		{
			m_mins.x = point.x;
		}
		else if (point.x > m_maxs.x)
		{
			m_maxs.x = point.x;
		}

		if (point.y < m_mins.y)
		{
			m_mins.y = point.y;
		}
		else if (point.y > m_maxs.y)
		{
			m_maxs.y = point.y;
		}

	}
}

void AABB2::SetPosAsBottomLeft(Vec2 const& pos)
{
	float width = GetDimensions().x;
	float length = GetDimensions().y;

	SetCenter(pos + Vec2(width * 0.5f, length * 0.5f));
}

void AABB2::SetPosAsBottomRight(Vec2 const& pos)
{
	float width = GetDimensions().x;
	float length = GetDimensions().y;

	SetCenter(pos + Vec2(-width * 0.5f, length * 0.5f));
}

void AABB2::SetPosAsTopRight(Vec2 const& pos)
{
	float width = GetDimensions().x;
	float length = GetDimensions().y;

	SetCenter(pos + Vec2(-width * 0.5f, -length * 0.5f));
}

void AABB2::SetPosAsTopLeft(Vec2 const& pos)
{
	float width = GetDimensions().x;
	float length = GetDimensions().y;

	SetCenter(pos + Vec2(width * 0.5f, -length * 0.5f));
}
