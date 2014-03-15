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

	int GetScoreValue() const;
	void SetScoreValue( const int scoreValue );

protected:
	sf::Time mTriggerNextDecision;
	float mEnemySpeed;

	sf::Vector2f mDecidedDirection;

	int mScoreValue;
};