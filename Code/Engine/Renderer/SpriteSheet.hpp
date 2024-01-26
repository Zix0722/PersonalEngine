#pragma once
#include <vector>
#include "SpriteDefinition.hpp"
#include "../Math/IntVec2.hpp"


struct Vec2;
struct AABB2;
class Texture;


class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout);
	explicit SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout, IntVec2 const& resolusion);
	explicit SpriteSheet(Texture& textrue, IntVec2 const& simpleGridLayout, float ratio);

	Texture&				GetTexture() const;
	int						GetNumSprite() const;
	SpriteDefinition const& GetSpriteDef(int spriteIndex) const;
	void					GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2					GetSpriteUVs(int spriteIndex) const;
	IntVec2						m_size;

protected:
	Texture&					m_texture;
	std::vector<SpriteDefinition>	m_spriteDefs;
};