#include "Enemy.hpp"

#include "Game.hpp"

Enemy::Enemy()
	:
	sf::Sprite(),
	mEnemySpeed(50.0f)
	{

	}

Enemy::~Enemy()
	{

	}

void Enemy::Update()
	{
	const sf::Time & frameDelta = gpGame->GetFrameDelta();
	sf::Vector2f moveDir( 0.0f, 1.0f );
	switch ( GetEvadeDirection() )
		{
		case EvadeDir::Null:
			{
			moveDir.y = 1.0f;
			break;
			}
		case EvadeDir::Left:
			{
			moveDir.x = -1.0f;
			break;
			}
		case EvadeDir::Right:
			{
			moveDir.x = 1.0f;
			break;
			}
		default:
			{
			throw std::runtime_error("Evade direction is not a valid value.");
			}
		}

	move( moveDir * mEnemySpeed * frameDelta.asSeconds() );
	}

sf::Time Enemy::GetTriggerNextDecision() const
	{
	return mTriggerNextDecision;
	}

void Enemy::SetTriggerNextDecision( sf::Time timeTrigger )
	{
	mTriggerNextDecision = timeTrigger;
	}

EvadeDir Enemy::GetEvadeDirection() const
	{
	// Get first shot in front of this enemy
	auto lasersPlayer = gpGame->GetLasersPlayer();

	const sf::Vector2f & enemyPos = getPosition();
	const sf::FloatRect & enemyRect = getGlobalBounds();

	// create rect that if the laser is in must be evaded
	sf::FloatRect evadeAreaOfEffect = enemyRect;
	float spaceToAdd = evadeAreaOfEffect.width * 0.5f;
	evadeAreaOfEffect.width += spaceToAdd;
	evadeAreaOfEffect.left -= spaceToAdd / 2.0f;
	evadeAreaOfEffect.height = gpGame->GetWindow().getSize().y - evadeAreaOfEffect.top;

	for ( auto & laser : lasersPlayer )
		{
		const sf::FloatRect & laserRect( laser.getGlobalBounds() );

		if ( evadeAreaOfEffect.intersects( laserRect ) )
			{
			// must evade this object

			// determine which direction to evade.
			float enemyMidpointX = enemyRect.left + (enemyRect.width / 2.0f);
			if ( enemyMidpointX <= laserRect.left )
				{
				// laser is on right half, so evade left
				return EvadeDir::Left;
				}
			else
				{
				return EvadeDir::Right;
				}
			}
		}

	// if we got here no lasers were heading in this enemies direction so keep going
	return EvadeDir::Null;
	}