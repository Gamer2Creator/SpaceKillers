#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

class Player : public sf::Sprite
{
public:
	Player();
	virtual ~Player();

	void Update();

protected:

	float mHealthMax;
	float mHealth;

	sf::Time mTriggerShootAgain;
};

