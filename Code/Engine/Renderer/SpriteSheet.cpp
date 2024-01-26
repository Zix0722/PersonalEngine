#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"

SpriteSheet::SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout)
	:m_texture(textrue)
	,m_size(simpleGridLayout)
{
	float uPerSpriteX = 1.f / (float)simpleGridLayout.x;
	float vPerSpriteY = 1.f / (float)simpleGridLayout.y;
	int spriteIndex = 0;
	for (int spriteY = 0; spriteY < simpleGridLayout.y; ++spriteY)
	{
		for (int spriteX = 0; spriteX < simpleGridLayout.x; ++spriteX)
		{
			float minU = uPerSpriteX * (float)spriteX;
			float maxU = minU + uPerSpriteX;
			float maxV = 1.f - (vPerSpriteY * (float)spriteY);
			float minV = maxV - vPerSpriteY;
			m_spriteDefs.push_back(SpriteDefinition(*this, spriteIndex,Vec2(minU, minV), Vec2(maxU, maxV)));
			++spriteIndex;
		}
	}
}

SpriteSheet::SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout, IntVec2 const& resolusion)
	:m_texture(textrue)
	, m_size(simpleGridLayout)
{
	float uPerSpriteX = 1.f / (float)simpleGridLayout.x;
	float vPerSpriteY = 1.f / (float)simpleGridLayout.y;
	int spriteIndex = 0;

	float minValueU = 1.f / (128 * resolusion.x);
	float minValueV = 1.f / (128 * resolusion.y);

	for (int spriteY = 0; spriteY < simpleGridLayout.y; ++spriteY)
	{
		for (int spriteX = 0; spriteX < simpleGridLayout.x; ++spriteX)
		{
			float minU = uPerSpriteX * (float)spriteX;
			float maxU = minU + uPerSpriteX;
			float maxV = 1.f - (vPerSpriteY * (float)spriteY);
			float minV = maxV - vPerSpriteY;
			m_spriteDefs.push_back(SpriteDefinition(*this, spriteIndex, Vec2(minU + minValueU, minV + minValueV), Vec2(maxU - minValueU, maxV - minValueV)));
			++spriteIndex;
		}
	}
}

SpriteSheet::SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout, float ratio)
	:m_texture(textrue)
	, m_size(simpleGridLayout)
{
	float uPerSpriteX = 1.f / (float)simpleGridLayout.x;
	float vPerSpriteY = ratio * 1.f / (float)simpleGridLayout.y;
	int spriteIndex = 0;
	for (int spriteY = 0; spriteY < simpleGridLayout.y; ++spriteY)
	{
		for (int spriteX = 0; spriteX < simpleGridLayout.x; ++spriteX)
		{
			float minU = uPerSpriteX * (float)spriteX;
			float maxU = minU + uPerSpriteX;
			float maxV = 1.f - (vPerSpriteY * (float)spriteY);
			float minV = maxV - vPerSpriteY;
			m_spriteDefs.push_back(SpriteDefinition(*this, spriteIndex, Vec2(minU, minV), Vec2(maxU, maxV)));
			++spriteIndex;
		}
	}
}

Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprite() const
{
	return (int)m_spriteDefs.size();
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	m_spriteDefs[spriteIndex].GetUVs(out_uvAtMins, out_uvAtMaxs);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex].GetUVs();
}

