#pragma once

#include "SFML/Graphics/Sprite.hpp"

class Laser : public sf::Sprite
{
public:
	Laser(const float speed = 750);
	virtual ~Laser();

protected:
	float mSpeed;
};