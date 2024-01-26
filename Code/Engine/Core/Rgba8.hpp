#pragma once

//---------------------------------------------------
struct Rgba8
{
public:
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;

	static const Rgba8 WHITE;
	static const Rgba8 RED;
	static const Rgba8 GREEN;
	static const Rgba8 BLUE;
	static const Rgba8 BLACK;
	static const Rgba8 GREY;
	static const Rgba8 YELLOW;
	static const Rgba8 CYAN;
	static const Rgba8 MAGENTA;
	static const Rgba8 LIGHTBLUE;

public:
	Rgba8();
	Rgba8(unsigned char red, unsigned char green, unsigned char blue, unsigned char A = 255);
	~Rgba8();

	void SetFromText(const char* text);
	void GetAsFloats(float* colorAsFloats) const;

	bool		operator==(const Rgba8& compare) const;
};