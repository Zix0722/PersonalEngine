#include "RandomNumberGenerator.hpp"
#include <random>
#include "ThirdParty/Squirrel/RawNoise.hpp"

int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
	unsigned int randomBits = Get1dNoiseUint(m_position++, m_seed);
	return randomBits % maxNotInclusive;
}

int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
	int possibilities = 1 + maxInclusive - minInclusive;
	return minInclusive + RollRandomIntLessThan( possibilities );
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	unsigned int randomInt = Get1dNoiseUint(m_position++, m_seed);
	constexpr unsigned int MAX_RANDOM_UNIT = 0xFFFFFFFF;
	constexpr double ONE_OVER_RAND_MAX = 1.0 / double(MAX_RANDOM_UNIT);
	return float(ONE_OVER_RAND_MAX * double(randomInt));
}

float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = maxInclusive - minInclusive;
	return minInclusive + (RollRandomFloatZeroToOne() * range); 
}
