#pragma once
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

class Player : public sf::Sprite
{
public:
	Player();
	virtual ~Player();

	void Update();
	
	int GetScore() const;
	void AddScore( int addValue );
	void SetScore( int value );

protected:
	int mScore;

	float mHealthMax;
	float mHealth;

	sf::Time mTriggerShootAgain;
};

