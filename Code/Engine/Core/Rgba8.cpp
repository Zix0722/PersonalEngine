#include <Engine/Core/Rgba8.hpp>
#include "StringUtils.hpp"
#include "ErrorWarningAssert.hpp"
#include "../Math/MathUtils.hpp"


const Rgba8 Rgba8::WHITE = Rgba8(255, 255, 255, 255);
const Rgba8 Rgba8::RED = Rgba8(255, 0, 0, 255);
const Rgba8 Rgba8::GREEN = Rgba8(0, 255, 0, 255);
const Rgba8 Rgba8::BLUE = Rgba8(0, 0, 255, 255);
const Rgba8 Rgba8::BLACK = Rgba8(0, 0, 0, 255);
const Rgba8 Rgba8::GREY = Rgba8(128, 128, 128, 255);
const Rgba8 Rgba8::YELLOW = Rgba8(255, 255, 0, 255);
const Rgba8 Rgba8::CYAN = Rgba8(0, 255, 255, 255);
const Rgba8 Rgba8::MAGENTA = Rgba8(255, 0, 255, 255);
const Rgba8 Rgba8::LIGHTBLUE = Rgba8(0, 255, 255, 255);

Rgba8::Rgba8()
{
	
}


Rgba8::Rgba8(unsigned char red, unsigned char green, unsigned char blue, unsigned char A)
	: r(red)
	, g(green)
	, b(blue)
	, a(A)
{

}

Rgba8::~Rgba8()
{
}

void Rgba8::SetFromText(const char* text)
{
	std::string strText = text;

	Strings splitStrings = SplitStringOnDelimiter(text, ',');
	if (splitStrings.size() == 3)
	{
		r = (unsigned char)atoi(splitStrings[0].c_str());
		g = (unsigned char)atoi(splitStrings[1].c_str());
		b = (unsigned char)atoi(splitStrings[2].c_str());
		a = 255;
	}
	else if (splitStrings.size() == 4)
	{
		r = (unsigned char)atoi(splitStrings[0].c_str());
		g = (unsigned char)atoi(splitStrings[1].c_str());
		b = (unsigned char)atoi(splitStrings[2].c_str());
		a = (unsigned char)atoi(splitStrings[3].c_str());
	}

}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = RangeMap((float)r, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[1] = RangeMap((float)g, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[2] = RangeMap((float)b, 0.f, 255.f, 0.f, 1.f);
	colorAsFloats[3] = RangeMap((float)a, 0.f, 255.f, 0.f, 1.f);
}

bool Rgba8::operator==(const Rgba8& compare) const
{
	if (r != compare.r)
	{
		return false;
	}
	if (g != compare.g)
	{
		return false;
	}
	if (b != compare.b)
	{
		return false;
	}
	if (a != compare.a)
	{
		return false;
	}
	return true;
}
