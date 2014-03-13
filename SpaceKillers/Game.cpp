#include "Game.hpp"

#include <random>
#include <ctime>

std::mt19937 gTwister;

float Random::FloatBetween( float low, float high )
	{
	std::uniform_real_distribution<float> dist(low, high);
	return dist(gTwister);
	}

Game * gpGame = nullptr;

const std::string & Game::GetTexturesFolder()
	{
	static const std::string texturesFolder("../Resources/Textures/");
	return texturesFolder;
	}

Game::Game()
	:
	mWindow(),
	mFrameTimeStamp(),
	mFrameDelta(),
	mReturnValue(0)
	{
	if ( gpGame )
		throw std::runtime_error("Game singleton violated.");
	else
		gpGame = this;
	gTwister.seed( time(nullptr) );

	mWindow.create(sf::VideoMode(800, 600), "Space Killers");

	LoadGame();
	
	}

Game::~Game()
	{
	gpGame = nullptr;
	}

void Game::Update()
	{
	UpdateBackground();
	UpdatePlayer();
	UpdateEnemies();
	UpdateLasers();
	}

void Game::Draw()
	{
	sf::RenderStates rstates(sf::BlendMode::BlendAlpha);
	mWindow.draw(mBackground1, rstates);
	mWindow.draw(mBackground2, rstates);

	mWindow.draw(mPlayer, rstates);

	for(auto & enemy : mEnemies)
		{
		mWindow.draw(enemy, rstates);
		}

	for(auto & playerLaser : mLasersPlayer )
		{
		mWindow.draw(playerLaser, rstates);
		}
	for(auto & enemyLaser : mLasersEnemy)
		{
		mWindow.draw(enemyLaser, rstates);
		}
	}

void Game::MainLoop()
	{
	sf::Event evt;
	sf::Clock clock;
	mFrameTimeStamp = clock.getElapsedTime();
	while(mWindow.isOpen())
		{
		while(mWindow.pollEvent(evt))
			{
			if(evt.type == sf::Event::KeyPressed)
				{
				if(evt.key.code == sf::Keyboard::Escape)
					{
					mWindow.close();
					}
				}
			if(evt.type == sf::Event::Closed)
				{
				mWindow.close();
				}
			}

		sf::Time now = clock.getElapsedTime();
		mFrameDelta = now - mFrameTimeStamp;
		mFrameTimeStamp = now;
		Update();
		Draw();
		mWindow.display();
		mWindow.clear(sf::Color(100, 100, 100, 255));
		}
	}

int Game::GetReturnValue() const
	{
	return mReturnValue;
	}

void Game::UpdateBackground()
	{
	// calculate how much to move the backgrounds by according to time passed.
	sf::Vector2f moveBy(0.0f, 1.0f);
	const float backgroundSpeed = 25.0f;
	moveBy *= backgroundSpeed * mFrameDelta.asSeconds();

	// figure out which one is on bottom and place the other the correct distance on top
	if(mBackground1.getPosition().y > mBackground2.getPosition().y)
		{
		mBackground1.move(moveBy);
		mBackground2.setPosition(0.0f, mBackground1.getGlobalBounds().top - mBackground2.getGlobalBounds().height);
		}
	else
		{
		mBackground2.move(moveBy);
		mBackground1.setPosition(0.0f, mBackground2.getGlobalBounds().top - mBackground1.getGlobalBounds().height);
		}

	// check if 1st background is off bottom screen if it is move it on top of 2nd
	if(mBackground1.getPosition().y > float(mWindow.getSize().y))
		{
		mBackground1.setPosition(mBackground2.getPosition() - sf::Vector2f(0.0f, float(mBackground1.getGlobalBounds().height)));
		}

	// check if 2nd background is off bottm screen, if it is move it on top of 1st
	if(mBackground2.getPosition().y > float(mWindow.getSize().y))
		{
		mBackground2.setPosition(mBackground1.getPosition() - sf::Vector2f(0.0f, float(mBackground2.getGlobalBounds().height)));
		}
	}

void Game::UpdatePlayer()
	{
	mPlayer.Update();
	}

void Game::UpdateEnemies()
	{
	// levels are handled by the score
	// base max enemies is 2, for every scoreRatio in points
	// another max enemy is added.
	int maxEnemies = 2;
	int scoreRatio = 1000;
	int playerScore = mPlayer.GetScore();
	
	maxEnemies += playerScore / scoreRatio;

	static float spawnTrigger = 0.0f;
	if ( spawnTrigger < mFrameTimeStamp.asSeconds() )
		{
		if(mEnemies.size() < maxEnemies )
			{
			Enemy newEnemy;
			newEnemy.setTexture(mEnemyShipTex);
			newEnemy.setScale(0.25f, 0.25f);
			newEnemy.setPosition(sf::Vector2f( Random::FloatBetween(0.0f, float(mWindow.getSize().x) - newEnemy.getGlobalBounds().width),-newEnemy.getGlobalBounds().height));
			mEnemies.emplace_back(newEnemy);
			}
		spawnTrigger = mFrameTimeStamp.asSeconds() + Random::FloatBetween(.5f, 5.f);
		}

	// update and delete enemy code
	for ( unsigned int i = 0; i < mEnemies.size(); ++i )
		{
		if ( mEnemies[i].getPosition().y > mWindow.getSize().y )
			{
			mEnemies.erase( mEnemies.begin() + i--);
			}
		else
			mEnemies[i].Update();
		}
	}

void Game::UpdateLasers()
	{
	// move lasers
	const float playerLaserSpeed = 500.0f;
	for(auto & laser : mLasersPlayer)
		{
		laser.move(0.0f, -playerLaserSpeed * mFrameDelta.asSeconds());
		}

	// Erase the lasers if they are off the play area completely on top and bottom
	for(int i = 0; i < mLasersPlayer.size(); ++i)
		{
		// top remove code
		const sf::FloatRect & laserBounds( mLasersPlayer[i].getGlobalBounds() );
		if(laserBounds.top + laserBounds.height < 0.0f)
			{
			mLasersPlayer.erase(mLasersPlayer.begin() + i--);
			continue;
			}

		// bottom remove code
		if(laserBounds.top > float(mWindow.getSize().y))
			{
			mLasersPlayer.erase(mLasersPlayer.begin() + i--);
			continue;
			}
		}
	}

void Game::LoadGame()
	{
	sf::Image backgroundImage;
	if(!backgroundImage.loadFromFile(GetTexturesFolder() + "nebula1.png"))
		{
		throw std::runtime_error("Failed to load image.");
		}
	// create texture on graphics card from image ( copies the image from ram to graphics card memory )
	if(!mBackgroundTex1.loadFromImage(backgroundImage))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// flip the image in ram
	backgroundImage.flipVertically();

	// copy the flipped image to graphics memory as another texture.
	if(!mBackgroundTex2.loadFromImage(backgroundImage))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// create and place background sprites
	mBackground1.setTexture(mBackgroundTex1);
	mBackground2.setTexture(mBackgroundTex2);
	mBackground2.setPosition(mBackground1.getPosition() + sf::Vector2f(0.0f, float(mBackground1.getGlobalBounds().height)));

	// load player texture
	if(!mPlayerShipTex.loadFromFile(GetTexturesFolder() + "Spaceship_tut.png"))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// setup player sprites texture scale and position
	mPlayer.setTexture(mPlayerShipTex);
	mPlayer.setScale(sf::Vector2f(0.25f, 0.25f));
	mPlayer.setPosition(sf::Vector2f(((mWindow.getSize().x / 2.0f) - float(mPlayer.getGlobalBounds().width) / 2.0f), mWindow.getSize().y - (mWindow.getSize().y / 4.0f)));

	// load players blue laser texture.
	if(!mLaserBlueTex.loadFromFile(GetTexturesFolder() + "laserBlue.png"))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// load enemies texture
	if(!mEnemyShipTex.loadFromFile(GetTexturesFolder() + "Titan.png"))
		{
		throw std::runtime_error("Failed to load image.");
		}

	UpdateEnemies(); // produce some enemies right away.
	}

void Game::CreateEnemyLaser(Enemy & enemy)
	{

	}

void Game::CreatePlayerLaser()
	{
	sf::Vector2f playerPos( mPlayer.getPosition() );
	float playerHalfWidth( mPlayer.getGlobalBounds().width / 2.0f );

	sf::Sprite laser(mLaserBlueTex);
	laser.setScale(0.25f,0.25f);

	sf::Vector2f startingLaserPos;
	startingLaserPos.x = playerPos.x + playerHalfWidth - (laser.getGlobalBounds().width / 2.0f);
	startingLaserPos.y = playerPos.y - laser.getGlobalBounds().height - 2.0f;
	laser.setPosition( startingLaserPos );
	mLasersPlayer.emplace_back( std::move( laser ) );
	}

const sf::RenderWindow & Game::GetWindow() const
	{
	return mWindow;
	}

sf::Time Game::GetFrameTimeStamp() const
	{
	return mFrameTimeStamp;
	}

sf::Time Game::GetFrameDelta() const
	{
	return mFrameDelta;
	}

const std::vector<sf::Sprite> & Game::GetLasersPlayer() const
	{
	return mLasersPlayer;
	}