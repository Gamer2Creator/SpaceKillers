#include "Player.hpp"

#include "Game.hpp"

#include "SFML/Window/Keyboard.hpp"

Player::Player()
	:
	mScore{0},
	mHealthMax{100.0f},
	mHealth{mHealthMax}
	{

	}

Player::~Player()
	{

	}

void Player::Update()
	{
	sf::Time frameDelta {gpGame->GetFrameDelta()};
	sf::Time frameTimeStamp {gpGame->GetFrameTimeStamp()};
	const float playerSpeed = 250.0f;
	float playerBoost = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ? 1.5f : 1.0f;

	sf::Vector2f playerPos {getPosition()};
	sf::Vector2f moveVec {0.0f, 0.0f};

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
		moveVec.y -= 1.0f;
		}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
		moveVec.y += 1.0f;
		}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
		moveVec.x += 1.0f;
		}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
		moveVec.x -= 1.0f;
		}

	playerPos += (moveVec * playerSpeed * playerBoost * frameDelta.asSeconds());

	sf::Vector2u windowSize {gpGame->GetWindow().getSize()};
	sf::FloatRect playerBounds {getGlobalBounds()};

	// keep player inside screen edge code.
	playerPos.x = std::min(float(windowSize.x) - float(playerBounds.width), playerPos.x);
	playerPos.x = std::max(0.0f, playerPos.x);
	playerPos.y = std::min(float(windowSize.y) - float(playerBounds.height), playerPos.y);
	playerPos.y = std::max(0.0f, playerPos.y);
	
	setPosition(playerPos);

	// shoot code. 
	sf::Time timeBetweenShots {sf::seconds(.50f)};
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
		if(mTriggerShootAgain <= frameTimeStamp)
			{
			gpGame->CreatePlayerLaser();
			AddScore(-10); // each shot removes 10 points.

			mTriggerShootAgain = frameTimeStamp + timeBetweenShots;
			}
		}
	}

int Player::GetScore() const
	{
	return mScore;
	}

void Player::AddScore( int addScore )
	{
	mScore += addScore;

	mScore = std::max(mScore, 0);
	}

void Player::SetScore( int setScore )
	{
	mScore = setScore;

	mScore = std::max(mScore, 0);
	}