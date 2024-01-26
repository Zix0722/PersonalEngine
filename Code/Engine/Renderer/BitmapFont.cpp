#include "BitmapFont.hpp"
#include "../Math/IntVec2.hpp"
#include "../Core/EngineCommon.hpp"
#include "../Math/AABB2.hpp"
#include "../Math/MathUtils.hpp"
#include "../Core/ErrorWarningAssert.hpp"
#include "../Core/VertexUtils.hpp"


BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
	:m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension)
	,m_fontGlyphsSpriteSheet(fontTexture, IntVec2(16, 16))
{
	
}

BitmapFont::~BitmapFont()
{

}

Texture& BitmapFont::GetTexture()
{
	return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/, int maxGlyphsToDraw)
{
	for (int charIndex = 0; charIndex < text.size(); ++charIndex)
	{
		char character = text[charIndex];
		float cellWidth = cellAspect * cellHeight;
		Vec2 textMaxs(cellWidth, cellHeight);
		textMaxs += textMins;

		AABB2 charBox(textMins, textMaxs);
		charBox.Translate(Vec2(static_cast<float>(charIndex) * cellWidth, 0.f));

		Vec2 uvMin(0.f, 0.f);
		Vec2 uvMax(1.f, 1.f);
		m_fontGlyphsSpriteSheet.GetSpriteUVs(uvMin, uvMax, character);

		Rgba8 trueTint = tint;
		if (charIndex > maxGlyphsToDraw - 1)
		{
			trueTint.a = 0;
		}

		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_mins.x, charBox.m_mins.y, 0.f), trueTint, uvMin));
		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_maxs.x, charBox.m_mins.y, 0.f), trueTint, Vec2(uvMax.x, uvMin.y)));
		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_maxs.x, charBox.m_maxs.y, 0.f), trueTint, uvMax));

		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_mins.x, charBox.m_mins.y, 0.f), trueTint, uvMin));
		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_maxs.x, charBox.m_maxs.y, 0.f), trueTint, uvMax));
		vertexArray.push_back(Vertex_PCU(Vec3(charBox.m_mins.x, charBox.m_maxs.y, 0.f), trueTint, Vec2(uvMin.x, uvMax.y)));

	}
}



void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/, Vec2 const& alignment /*= Vec2(.5f, .5f)*/, TextBoxMode mode /*= TextBoxMode::SHRINK_TO_FIT*/, int maxGlyphsToDraw /*= 99999999*/)
{
	if (mode == TextBoxMode::SHRINK_TO_FIT)
	{
		Vec2 boxDimensions = box.GetDimensions();
		float boxWidth = boxDimensions.x;
		float boxHeight = boxDimensions.y;

		Strings wholeStr;
		wholeStr = SplitStringOnDelimiter(text, '\n');

		int numLines = (int)wholeStr.size();
		float sizeFactor = 1.0f;

		for (int strIndex = 0; strIndex < numLines; ++strIndex)
		{
			if (GetTextWidth(cellHeight, wholeStr[strIndex], cellAspect) > boxWidth)
			{
				sizeFactor = boxWidth / GetTextWidth(cellHeight, wholeStr[strIndex], cellAspect);
			}
		}

		for (int strIndex = 0; strIndex < numLines; ++strIndex)
		{
			int factor = numLines - strIndex - 1;
			Vec2 charBoxMin = box.m_mins;
			float freeVertical = boxHeight;
			freeVertical -= numLines * cellHeight * sizeFactor;
			charBoxMin.y += freeVertical * alignment.y + factor * cellHeight * sizeFactor;
			float freeHoriz = boxWidth;
			freeHoriz -= GetTextWidth(cellHeight * sizeFactor, wholeStr[strIndex], cellAspect);
			charBoxMin.x += freeHoriz * alignment.x;

			int remainGlyohsToGraw = maxGlyphsToDraw;
			for (int index = 0; index < strIndex; ++index)
			{
				if (strIndex != 0)
				{
					remainGlyohsToGraw = remainGlyohsToGraw - (int)wholeStr[index].length();
				}
			}
			AddVertsForText2D(vertexArray, charBoxMin, cellHeight * sizeFactor, wholeStr[strIndex], tint, cellAspect, remainGlyohsToGraw);
		}


	}
	if (mode == TextBoxMode::OVERRUN)
	{
		Vec2 boxDimensions = box.GetDimensions();
		float boxWidth = boxDimensions.x;
		float boxHeight = boxDimensions.y;

		Strings wholeStr;
		wholeStr = SplitStringOnDelimiter(text, '\n');

		int numLines = (int)wholeStr.size();
		
		for (int strIndex = 0; strIndex < numLines; ++strIndex)
		{
			
			int factor = numLines - strIndex -1;
			Vec2 charBoxMin = box.m_mins;
			float freeVertical = boxHeight;
			freeVertical -= numLines * cellHeight;
			charBoxMin.y += freeVertical * alignment.y + factor * cellHeight;
			float freeHoriz = boxWidth;
			freeHoriz -= GetTextWidth(cellHeight, wholeStr[strIndex], cellAspect);
			charBoxMin.x += freeHoriz * alignment.x;

			int remainGlyohsToGraw = maxGlyphsToDraw;
			for (int index = 0; index < strIndex; ++index)
			{
				if (strIndex != 0)
				{
					remainGlyohsToGraw = remainGlyohsToGraw - (int)wholeStr[index].length();
				}
			}
			AddVertsForText2D(vertexArray, charBoxMin, cellHeight, wholeStr[strIndex], tint, cellAspect, remainGlyohsToGraw);
		}
	}
}

void BitmapFont::AddVertsForText3D(std::vector<Vertex_PCU>& verts, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint /*= Rgba8::WHITE*/, float cellAspect /*= 1.f*/, Vec2 const& alignment /*= Vec2(.5f, .5f)*/, int maxGlyphsToDraw /*= 99999999*/)
{
	std::vector<Vertex_PCU> localVerts;
	AddVertsForText2D(localVerts, textMins, cellHeight, text, tint, cellAspect, maxGlyphsToDraw);
	AABB2 bounds = GetVertexBounds2D(localVerts);
	Vec2 dimensions = bounds.GetDimensions();
	float xOffset = textMins.x - dimensions.x * alignment.x;
	float yOffset = textMins.y - dimensions.y * alignment.y;

	AddVertsForText2D(verts, Vec2(xOffset, yOffset), cellHeight, text, tint, cellAspect, maxGlyphsToDraw);
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect /*= 1.f*/)
{
	float cellWidth = cellAspect * cellHeight;
	float stringWidth = cellWidth * text.length();
	
	return stringWidth;
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);

	return 1.f;
}

