#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

enum class EvadeDir
	{
	Null,
	Left,
	Right,
	End
	};

class Enemy : public sf::Sprite
{
public:
	Enemy();
	virtual ~Enemy();

	virtual void Update();

	sf::Time GetTriggerNextDecision() const;
	void SetTriggerNextDecision(sf::Time timeTrigger);

	EvadeDir GetEvadeDirection() const;
	void StayInBounds();

	int GetScoreValue() const;
	void SetScoreValue( const int scoreValue );

	void Shoot();

protected:
	sf::Time mTriggerNextDecision;
	float mEnemySpeed;

	sf::Vector2f mDecidedDirection;

	sf::Time mTriggerCanShoot;
	sf::Time mTriggerNextRandomShot;

	int mScoreValue;
};