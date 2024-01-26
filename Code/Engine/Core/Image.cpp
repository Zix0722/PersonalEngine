#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/Image.hpp"
#include "ErrorWarningAssert.hpp"
#include "StringUtils.hpp"

Image::Image(const char* imageFilePath) :
	m_imageFilePath(imageFilePath)
{
	int imageTexelSizeX = 0; 
	int imageTexelSizeY = 0; 
	int numComponents = 0; //(3=RGB=24bit, 4=RGBA=32bit)
	int numComponentsRequested = 0; 

	stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	unsigned char* imageData = stbi_load(imageFilePath, &imageTexelSizeX, &imageTexelSizeY, &numComponents, numComponentsRequested);

	GUARANTEE_OR_DIE(imageData, Stringf("Failed to load image \"%s\"", imageFilePath));
	//GUARANTEE_OR_DIE(numComponents >= 3 && numComponents <= 4 && imageTexelSizeX > 0 && imageTexelSizeY > 0, Stringf("ERROR loading image \"%s\" (Bpp=%i, size=%i,%i)", imageFilePath, numComponents, imageTexelSizeX, imageTexelSizeY));
	

	m_dimensions = IntVec2(imageTexelSizeX, imageTexelSizeY);

	if (numComponents == 3)
	{
		for (int y = 0; y < m_dimensions.y; y++)
		{
			for (int x = 0; x < m_dimensions.x * numComponents; x += 3)
			{
				int currentIndex = y * m_dimensions.x * numComponents + x;
				unsigned char r = imageData[currentIndex];
				unsigned char g = imageData[currentIndex + 1];
				unsigned char b = imageData[currentIndex + 2];
				Rgba8 rgbaTexel(r, g, b);
				m_rgbaTexels.push_back(rgbaTexel);
			}
		}
	}
	else if (numComponents == 4)
	{
		for (int y = 0; y < m_dimensions.y; y++)
		{
			for (int x = 0; x < m_dimensions.x * numComponents; x += 4)
			{
				int currentIndex = y * m_dimensions.x * numComponents + x;
				unsigned char r = imageData[currentIndex];
				unsigned char g = imageData[currentIndex + 1];
				unsigned char b = imageData[currentIndex + 2];
				unsigned char a = imageData[currentIndex + 3];
				Rgba8 rgbaTexel(r, g, b, a);
				m_rgbaTexels.push_back(rgbaTexel);
			}
		}
	}
	else if (numComponents == 1)
	{
		for (int y = 0; y < m_dimensions.y; y++)
		{
			for (int x = 0; x < m_dimensions.x * numComponents; x += 1)
			{
				int currentIndex = y * m_dimensions.x * numComponents + x;
				unsigned char r = imageData[currentIndex];
				unsigned char g = imageData[currentIndex];
				unsigned char b = imageData[currentIndex];
				Rgba8 rgbaTexel(r, g, b);
				m_rgbaTexels.push_back(rgbaTexel);
			}
		}
	}
	// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	stbi_image_free(imageData);
}

Image::Image()
{

}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimensions(size)
{
	for (int texelIndex = 0; texelIndex < m_dimensions.x * m_dimensions.y; ++texelIndex)
	{
		m_rgbaTexels.push_back(color);
	}
}

Image::~Image()
{

}

const std::string& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}


Rgba8 Image::GetTexelColor(const IntVec2& texelCoords) const
{
	int texelIndex = texelCoords.y * m_dimensions.x + texelCoords.x;
	return m_rgbaTexels[texelIndex];
}

void Image::SetTexelColor(const Rgba8& color, const IntVec2& texelCoords)
{
	int texelIndex = texelCoords.y * m_dimensions.x + texelCoords.x;
	m_rgbaTexels[texelIndex] = color;
}

void const* Image::GetRawData() const
{
	return m_rgbaTexels.data();
}
