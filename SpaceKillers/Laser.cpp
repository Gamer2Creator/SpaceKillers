#include "Laser.hpp"

Laser::Laser(const float speed)
	:
	mSpeed(speed)
	{

	}

Laser::~Laser()
	{

	}

float Laser::GetSpeed() const
	{
	return mSpeed;
	}

void Laser::SetSpeed(const float speed )
	{
	mSpeed = speed;
	}