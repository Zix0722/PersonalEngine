#pragma once

//-------------------------------------------------------
class RandomNumberGenerator
{
public:
	int		RollRandomIntLessThan(int maxNotInclusive);
	int		RollRandomIntInRange(int minInclusive, int maxInclusive);
	float	RollRandomFloatZeroToOne();
	float	RollRandomFloatInRange(float minInclusive, float maxInclusive);



public:
	  unsigned int			m_seed = 0;			// we will use these later on...
	  int					m_position = 0;		//,,, when we replace rand() with noise 

};