#include "Enemy.hpp"

#include "Game.hpp"

Enemy::Enemy()
	:
	sf::Sprite(),
	mEnemySpeed(75.0f),
	mScoreValue(100)
	{

	}

Enemy::~Enemy()
	{

	}

void Enemy::Update()
	{
	// get time information
	const sf::Time & frameDelta = gpGame->GetFrameDelta();
	const sf::Time & frameStamp = gpGame->GetFrameTimeStamp();

	// get the direction to evade if there is nothing to evade EvadeDir::Null is returned.
	const EvadeDir evadeDir = GetEvadeDirection();
	const sf::FloatRect enemyRect( getGlobalBounds() );
	const sf::Vector2u windowSize( gpGame->GetWindow().getSize() );
	const sf::FloatRect playerRect( gpGame->GetPlayer().getGlobalBounds() );

	// this rect represents the area the enemy considers the player in his sights.
	const float growWidth = 2.0f * enemyRect.width;
	
	// creating the rect which describes the area of attack, which is the area the enemy is aware of and will seek the player within.
	const sf::FloatRect areaOfAttack(enemyRect.left - (growWidth / 2.0f),
									 enemyRect.top,
									 enemyRect.width + growWidth,
									 windowSize.y - enemyRect.top );

	bool isPlayerInAreaOfAttack = areaOfAttack.intersects(playerRect);

	// check to evade, this trumps all decisions
	// if a laser is coming at this enemy it will attempt evade
	if ( evadeDir != EvadeDir::Null )
		{
		// need to evade in a particular direction
		sf::Vector2f evadeVec(0.0f, mDecidedDirection.y);
		if(evadeDir == EvadeDir::Left)
			{
			evadeVec.x = -1.0f;
			}
		if(evadeDir == EvadeDir::Right)
			{
			evadeVec.x = 1.0f;
			}
		// move in this direction and keep the decided forward speed.
		move( evadeVec * mEnemySpeed * frameDelta.asSeconds() );
		StayInBounds();
		}
	else // if there is no laser incoming then we can do other behaviors
		{
		// check for next decision
		if ( mTriggerNextDecision <= frameStamp )
			{
			// if player is in area of attack while making a new decision then the decision
			// should be to move in player direction
			if ( isPlayerInAreaOfAttack )
				{
				// calculate to move left or rigth depending on relative player position
				if(areaOfAttack.left + (areaOfAttack.width / 2.0f) <= playerRect.left + (playerRect.width / 2.0f))
					{
					mDecidedDirection.x = Random::FloatBetween(0.5f, 1.0f);
					mDecidedDirection.y = Random::FloatBetween(0.5f, 1.0f);
					}
				else
					{
					mDecidedDirection.x = -Random::FloatBetween(0.5f, 1.0f);
					mDecidedDirection.y = Random::FloatBetween(0.5f, 1.0f);
					}
				}
			else
				{
				// player is not in area of attack, do random directions and speed.
				mDecidedDirection.x = Random::FloatBetween(-1.0f, 1.0f);
				mDecidedDirection.y = Random::FloatBetween(0.5f, 1.0f);
				}
				
			mTriggerNextDecision = frameStamp + sf::seconds(Random::FloatBetween(0.5f, 2.0f));
			}
		move(mDecidedDirection * mEnemySpeed * frameDelta.asSeconds());
		StayInBounds();
		}

	// shooting code.
	// if the player is in the area of attack
	if ( isPlayerInAreaOfAttack )
		{
		// shoot as fast as it can
		Shoot();
		mTriggerNextRandomShot = frameStamp + sf::seconds(Random::FloatBetween(.5f, 3.5f));
		}
	else
		{
		// shoot random slower shots
		if(mTriggerNextRandomShot <= frameStamp)
			{
			Shoot();
			mTriggerNextRandomShot = frameStamp + sf::seconds(Random::FloatBetween(.5f, 3.5f));
			}
		}

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

	const sf::FloatRect & enemyRect = getGlobalBounds();

	// create rect that if the laser's rect touches, that laser should be evaded
	sf::FloatRect evadeAreaOfEffect = enemyRect; // set to the enemy rect initially
	float scaleRatio = 1.0f; // the ratio to scale by
	float widthToAdd = evadeAreaOfEffect.width * scaleRatio; // the calculated width to add
	evadeAreaOfEffect.width += widthToAdd; // add extra width
	evadeAreaOfEffect.left -= widthToAdd / 2.0f; // move rect left by half of added width so it's centered.
	evadeAreaOfEffect.height = gpGame->GetWindow().getSize().y - evadeAreaOfEffect.top; // grow height of rect to bottom of screen.

	for ( auto & laser : lasersPlayer )
		{
		const sf::FloatRect & laserRect( laser.getGlobalBounds() );

		// using intersects to detect collision between the area we have created and the laser rect.
		if ( evadeAreaOfEffect.intersects( laserRect ) )
			{
			// must evade this object

			// determine which direction to evade.
			float enemyMidpointX = enemyRect.left + (enemyRect.width / 2.0f);
			if ( enemyMidpointX <= laserRect.left + (laserRect.width / 2.0f) )
				{
				// laser is on right half, so evade left.
				return EvadeDir::Left;
				}
			else
				{
				// laser is on left half, so evade right.
				return EvadeDir::Right;
				}
			}
		}

	// if we got here no lasers were heading in this enemies direction so keep going
	return EvadeDir::Null;
	}

void Enemy::StayInBounds()
	{
	// check out of bounds tell enemy to move inwards
	const sf::FloatRect newEnemyRect = getGlobalBounds();
	const float minSpeedForEdgeAvoid = .5f;

	// check for off left
	if(newEnemyRect.left < 0.0f)
		{
		mDecidedDirection.x = std::max( minSpeedForEdgeAvoid, std::abs(mDecidedDirection.x));
		}
	// check for off right
	if(newEnemyRect.left + newEnemyRect.width >= gpGame->GetWindow().getSize().x)
		{
		mDecidedDirection.x = -std::max(minSpeedForEdgeAvoid, std::abs(mDecidedDirection.x));
		}
	}

int Enemy::GetScoreValue() const
	{
	return mScoreValue;
	}

void Enemy::SetScoreValue( const int scoreValue )
	{
	mScoreValue = scoreValue;
	}

void Enemy::Shoot()
	{
	if ( mTriggerCanShoot <= gpGame->GetFrameTimeStamp() )
		{
		gpGame->CreateEnemyLaser(*this);
		mTriggerCanShoot = gpGame->GetFrameTimeStamp() + sf::seconds(Random::FloatBetween(.5f, 1.0f));
		}
	}