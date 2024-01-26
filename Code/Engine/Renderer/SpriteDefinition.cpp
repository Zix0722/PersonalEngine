#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"


SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
	:m_spriteSheet(spriteSheet)
	,m_spriteIndex(spriteIndex)
	,m_uvAtMins(uvAtMins)
	,m_uvAtMaxs(uvAtMaxs)
{

}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const
{
	out_uvAtMins = Vec2(m_uvAtMins.x, m_uvAtMins.y);
	out_uvAtMaxs = Vec2(m_uvAtMaxs.x, m_uvAtMaxs.y);
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(Vec2(m_uvAtMins.x, m_uvAtMins.y)
			   , Vec2(m_uvAtMaxs.x, m_uvAtMaxs.y));
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

Texture& SpriteDefinition::GetTexture() const
{
	return m_spriteSheet.GetTexture();
}

float SpriteDefinition::GetAspect() const
{
	float xDisplacement = m_uvAtMaxs.x - m_uvAtMins.x;
	float yDisplacement = m_uvAtMaxs.y - m_uvAtMins.y;
	float aspect = xDisplacement / yDisplacement;

	return aspect;
}

