#pragma once

#include "SFML/Graphics/Sprite.hpp"

class Laser : public sf::Sprite
{
public:
	Laser(const float speed = 750.0f);
	virtual ~Laser();

	float GetSpeed() const;
	void SetSpeed( const float speed );

protected:
	float mSpeed;
};