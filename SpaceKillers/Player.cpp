#include "Player.hpp"

#include "Game.hpp"

#include "SFML/Window/Keyboard.hpp"

Player::Player()
	:
	mScore(0),
	mHealthMax(100.0f),
	mHealth(mHealthMax)
	{

	}


Player::~Player()
	{
	}

void Player::Update()
	{
	const float playerSpeed = 100.0f;
	float playerBoost;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		{
		playerBoost = 2.0f;
		}
	else
		{
		playerBoost = 1.0f;
		}

	sf::Vector2f  playerPos = getPosition();
	sf::Vector2f moveVec(0.0f, 0.0f);
	float frameDelta = gpGame->GetFrameDelta().asSeconds();
	float frameTimeStamp = gpGame->GetFrameTimeStamp().asSeconds();

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
		moveVec.y -= 1.0f;
		}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
		moveVec.y += 1.0f;
		}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
		moveVec.x += 1.0f;
		}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
		moveVec.x -= 1.0f;
		}

	playerPos += (moveVec * playerSpeed * playerBoost * frameDelta);

	sf::Vector2u windowSize = gpGame->GetWindow().getSize();
	sf::FloatRect playerBounds = getGlobalBounds();

	// keep player inside screen edge code.
	playerPos.x = std::min(float(windowSize.x) - float(playerBounds.width), playerPos.x);
	playerPos.x = std::max(0.0f, playerPos.x);
	playerPos.y = std::min(float(windowSize.y) - float(playerBounds.height), playerPos.y);
	playerPos.y = std::max(0.0f, playerPos.y);
	
	setPosition(playerPos);

	// shoot code. 
	static float shootAgainTrigger = 0.0f;
	static const float timeBetweenShots = .50f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
		if(shootAgainTrigger <= frameTimeStamp)
			{
			gpGame->CreatePlayerLaser();

			shootAgainTrigger = frameTimeStamp + timeBetweenShots;
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