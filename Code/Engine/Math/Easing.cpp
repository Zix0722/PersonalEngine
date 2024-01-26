#include "Engine/Math/MathUtils.hpp"
#include <corecrt_math.h>
#include "Easing.hpp"


float SmoothStart2(float input)
{
	return input * input;
}

float SmoothStart3(float input)
{
	return input * input * input;
}

float SmoothStart4(float input)
{
	return input * input * input * input;
}

float SmoothStart5(float input)
{
	return input * input * input * input * input;
}

float SmoothStart6(float input)
{
	return input * input * input * input * input * input;
}

float SmoothStop2(float input)
{
	float t = 1 - input;
	return 1 - t * t;
}

float SmoothStop3(float input)
{
	float t = 1 - input;
	return 1 - t * t * t;
}

float SmoothStop4(float input)
{
	float t = 1 - input;
	return 1 - t * t * t * t;
}

float SmoothStop5(float input)
{
	float t = 1 - input;
	return 1 - t * t * t * t * t;
}

float SmoothStop6(float input)
{
	float t = 1 - input;
	return 1 - t * t * t * t * t * t;
}

float SmoothStep3(float input)
{
	return ComputeCubicBezier1D(0.f, 0.f, 1.f, 1.f, input);
}

float SmoothStep5(float input)
{
	return ComputeQuinticBezier1D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, input);

}

float Hesitate3(float input)
{
	return ComputeCubicBezier1D(0.f, 1.f, 0.f, 1.f, input);
}

float Hesitate5(float input)
{
	return ComputeQuinticBezier1D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f, input);
}

float FunckyFunction(float input)
{
	return 0.05f * sinf(20.f * input * sinf(5.f * input)) + 0.4f * input + 0.6f * input;
}

