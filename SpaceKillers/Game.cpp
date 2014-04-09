#include "Game.hpp"

#include "MathUtils.hpp"

#include <random>
#include <ctime>
 
std::mt19937 gTwister;

float Random::FloatBetween( float low, float high )
	{
	std::uniform_real_distribution<float> dist{low, high};
	return dist(gTwister);
	}

long long Random::LongBetween( long long low, long long high )
	{
	std::uniform_int_distribution<long long> dist{low,high};
	return dist(gTwister);
	}

Game * gpGame = nullptr;

const std::string & Game::GetTexturesFolder()
	{
	static const std::string texturesFolder {"../Resources/Textures/"};
	return texturesFolder;
	}

const std::string & Game::GetFontsFolder()
	{
	static const std::string fontsFolder {"../Resources/Fonts/"};
	return fontsFolder;
	}

const std::string & Game::GetSoundsFolder()
	{
	static const std::string soundsFolder {"../Resources/Sounds/"};
	return soundsFolder;
	}

Game::Game()
	:
	mWindow{},
	mCurrentState{State::MainMenu},
	mFrameTimeStamp{},
	mFrameDelta{},
	mFrameDeltaFixed{ sf::seconds(1.0f / 120.0f) },
	mReturnValue{},
	mBackgroundSpeed{25.f}
	{
	if ( gpGame )
		throw std::runtime_error{"Game singleton violated."};
	else
		gpGame = this;
	gTwister.seed( unsigned long(time(nullptr)) );

	mWindow.create(sf::VideoMode{800, 600}, "Space Killers");

	LoadGame();
	ChangeState(State::MainMenu);
	}

Game::~Game()
	{
	gpGame = nullptr;
	}

void Game::ResetGame()
	{
	mTextScoreBoard.SetScore(0);
	mPlayer.setPosition( GetPlayerSpawnPosition() );
	mEnemies.clear();
	mLasersPlayer.clear();
	mLasersEnemy.clear();
	mExplosions.clear();
	mTextTimeDisplay.Reset();
	}

void Game::UpdateState()
	{
	switch(mCurrentState)
		{
		case State::MainMenu:
			{
			UpdateStateMainMenu();
			break;
			}
		case State::Playing:
			{
			UpdateStatePlaying();
			break;
			}
		case State::Dead:
			{
			UpdateStateDead();
			break;
			}
		case State::Paused:
			{
			UpdateStatePaused();
			break;
			}
		default:
			{
			throw std::runtime_error("Unknown state, " + std::to_string(static_cast<int>(mCurrentState)) );
			break;
			}
		}
	}

void Game::DrawState()
	{
	switch(mCurrentState)
		{
		case State::MainMenu:
			{
			DrawStateMainMenu();
			break;
			}
		case State::Playing:
			{
			DrawStatePlaying();
			break;
			}
		case State::Dead:
			{
			DrawStateDead();
			break;
			}
		case State::Paused:
			{
			DrawStatePaused();
			break;
			}
		default:
			{
			throw std::runtime_error("Unknown state, " + std::to_string(static_cast<int>(mCurrentState)) );
			}
		}
	}

void Game::ChangeState( State state )
	{
	// this is where setup for states is done.
	switch ( state )
		{
		case State::MainMenu:
			{
			ResetGame();
			mTextInfo.setString("Space Killers!\nPress enter key to play.");
			sf::FloatRect welcomeRect{ mTextInfo.getGlobalBounds() };
			sf::Vector2f windowSize{ mWindow.getSize() };
			sf::Vector2f welcomeCenteredPos{};
			welcomeCenteredPos.x = float(windowSize.x / 2) - (welcomeRect.width / 2.0f);
			welcomeCenteredPos.y = float(windowSize.y / 2) - (welcomeRect.height / 2.0f);
			mTextInfo.setPosition(welcomeCenteredPos);
			break;
			}
		case State::Playing:
			{
			if ( mCurrentState == State::Dead )
				{
				ResetGame();
				break;
				}
			if ( mCurrentState == State::Paused )
				{
				break;
				}
			if ( mCurrentState == State::MainMenu )
				{
				mSoundManager.SetMusic( AudioMusic::Playing );
				break;
				}
			break;
			}
		case State::Dead:
			{
			mPlayer.setPosition(0.0, -mPlayer.getGlobalBounds().height * 2.0f);
			mTextInfo.setString("You died!\n\nEnter to play again\nEscape to exit.");
			sf::FloatRect welcomeRect{ mTextInfo.getGlobalBounds() };
			sf::Vector2f windowSize{ mWindow.getSize() };
			sf::Vector2f welcomeCenteredPos{};
			welcomeCenteredPos.x = float(windowSize.x / 2) - (welcomeRect.width / 2.0f);
			welcomeCenteredPos.y = float(windowSize.y / 2) - (welcomeRect.height / 2.0f);
			mTextInfo.setPosition(welcomeCenteredPos);
			break;
			}
		case State::Paused:
			{
			sf::Color color = mTextInfo.getColor();
			color.a = 255;
			mTextInfo.setColor(color);
			mTextInfo.setString("Press Enter to unpause.");
			sf::FloatRect infoRect{ mTextInfo.getGlobalBounds() };
			sf::Vector2f windowSize{ mWindow.getSize() };
			sf::Vector2f infoCenteredPos{};
			infoCenteredPos.x = float(windowSize.x / 2) - (infoRect.width / 2.0f);
			infoCenteredPos.y = float(windowSize.y / 2) - (infoRect.height / 2.0f);
			mTextInfo.setPosition(infoCenteredPos);
			break;
			}
		}

	mCurrentState = state;
	}

void Game::UpdateStatePlaying()
	{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Pause ) )
		{
		ChangeState(State::Paused);
		return;
		}
	UpdateBackground();
	UpdatePlayer();
	UpdateEnemies();
	UpdateLasers();
	UpdateExplosions();
	UpdateGUI(); 
	}

void Game::DrawStatePlaying()
	{
	sf::RenderStates rstates{sf::BlendMode::BlendAlpha};
	
	DrawBackgrounds(rstates);
	DrawPlayer(rstates);
	DrawEnemies(rstates);
	DrawLasers(rstates);
	DrawExplosions(rstates);
	DrawGUI(rstates);

	DrawGUI(rstates);
	}

void Game::UpdateStateMainMenu()
	{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Return ) )
		{
		ChangeState( State::Playing );
		}
	float frameStamp = mFrameTimeStamp.asSeconds();
	sf::Color textColor = mTextInfo.getColor();
	textColor.a = char( abs(255.0f * sin(frameStamp)) );
	mTextInfo.setColor(textColor);

	UpdateBackground();
	}

void Game::DrawStateMainMenu()
	{
	sf::RenderStates rstates{ sf::BlendMode::BlendAlpha };

	DrawBackgrounds(rstates);

	DrawInfoText(rstates);
	}

void Game::UpdateStateDead()
	{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Return ) )
		{
		ChangeState( State::Playing );
		}
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
		{
		mWindow.close();
		}

	float frameStamp = mFrameTimeStamp.asSeconds();
	sf::Color textColor = mTextInfo.getColor();
	textColor.a = char(abs(255.0f * sin(frameStamp)));
	mTextInfo.setColor(textColor);

	UpdateBackground();
	UpdateEnemies();
	UpdateLasers();
	UpdateExplosions();
	}

void Game::DrawStateDead()
	{
	sf::RenderStates rstates( sf::BlendMode::BlendAlpha );

	DrawBackgrounds(rstates);
	DrawLasers(rstates);
	DrawEnemies(rstates);
	DrawExplosions(rstates);
	DrawGUI(rstates);
	DrawInfoText(rstates);
	}

void Game::UpdateStatePaused()
	{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Return ) )
		{
		ChangeState(State::Playing);
		}
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Escape ) )
		{
		mWindow.close();
		}
	}

void Game::DrawStatePaused()
	{
	sf::RenderStates rstates{ sf::BlendMode::BlendAlpha };

	DrawBackgrounds(rstates);
	DrawPlayer(rstates);
	DrawEnemies(rstates);
	DrawLasers(rstates);
	DrawExplosions(rstates);
	DrawGUI(rstates);

	DrawGUI(rstates);
	DrawInfoText(rstates);
	}

void Game::MainLoop()
	{
	sf::Event evt{};
	sf::Clock clock{};
	mFrameTimeStamp = clock.getElapsedTime();
	sf::Time lastUpdateTimeStamp {mFrameTimeStamp};
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

		sf::Time now {clock.getElapsedTime()};
		mFrameDelta = now - mFrameTimeStamp;
		mFrameTimeStamp = now;

		if(lastUpdateTimeStamp + mFrameDeltaFixed <= mFrameTimeStamp)
			{
			UpdateState();
			
			lastUpdateTimeStamp = clock.getElapsedTime();
			}

		DrawState();
		mSoundManager.Update();

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
	sf::Vector2f moveBy {0.0f, 1.0f};
	moveBy *= mBackgroundSpeed * mFrameDeltaFixed.asSeconds();

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
	unsigned int maxEnemies = 2;
	int scoreRatio = 1000;
	int playerScore = mTextScoreBoard.GetScore();
	sf::Vector2u windowSize {mWindow.getSize()};
	
	maxEnemies += playerScore / scoreRatio;

	static float spawnTrigger = 0.0f;
	if ( spawnTrigger < mFrameTimeStamp.asSeconds() )
		{
		if(mEnemies.size() < maxEnemies )
			{
			Enemy newEnemy{};
			newEnemy.setTexture(texLoader.Get(TextureAsset::EnemyShip));
			newEnemy.setScale(0.25f, 0.25f);
			sf::FloatRect enemyRect {newEnemy.getGlobalBounds()};

			sf::Vector2f position {};
			position.x = Random::FloatBetween(0.0f, float(windowSize.x) - enemyRect.width );// random pos along x within bounds
			position.y = -newEnemy.getGlobalBounds().height; // move it above the window = to it's height, bottom is 1 pixel above screen

			newEnemy.setPosition(position);
			mEnemies.emplace_back(newEnemy);
			}
		spawnTrigger = mFrameTimeStamp.asSeconds() + Random::FloatBetween(.5f, 5.f);
		}

	// check for player enemy collision
	for ( unsigned int i = 0; i < mEnemies.size(); --i )
		{
		if ( mEnemies[i].getGlobalBounds().intersects( mPlayer.getGlobalBounds() ) )
			{
			CreateExplosionShip( mEnemies[i].getGlobalBounds() );
			CreateExplosionShip( mPlayer.getGlobalBounds() );
			mEnemies.erase( mEnemies.begin() + i-- );
			ChangeState(State::Dead);
			}
		}

	// update and delete enemy code
	for ( unsigned int i = 0; i < mEnemies.size(); ++i )
		{
		if ( mEnemies[i].getPosition().y > mWindow.getSize().y )
			{
			mEnemies.erase( mEnemies.begin() + i--);
			}
		else
			{
			mEnemies[i].Update();
			}
		}
	}

void Game::UpdateLasers()
	{
	const sf::Vector2u windowSize {mWindow.getSize()};
	// move lasers
	// Erase the lasers if they are off the play area completely on top and bottom
	// if not erased then move them.
	for(unsigned int i = 0; i < mLasersPlayer.size(); ++i)
		{
		// top remove code
		const sf::FloatRect & laserBounds {mLasersPlayer[i].getGlobalBounds()};
		if(laserBounds.top + laserBounds.height < 0.0f
			|| laserBounds.top > float(windowSize.y))
			{
			mLasersPlayer.erase(mLasersPlayer.begin() + i--); 
			mTextScoreBoard.AddScore(-10);
			continue;
			}
		mLasersPlayer[i].move(0.0f, -mLasersPlayer[i].GetSpeed() * mFrameDeltaFixed.asSeconds());
		}

	// remove enemy lasers when outside the window
	for(unsigned int i = 0; i < mLasersEnemy.size(); ++i )
		{
		const sf::FloatRect & laserBounds {mLasersEnemy[i].getGlobalBounds()};
		if(laserBounds.top + laserBounds.height < 0.0f )
			{
			mLasersEnemy.erase(mLasersEnemy.begin() + i--);
			continue;
			}
		if ( laserBounds.top > float(windowSize.y) )
			{
			mLasersEnemy.erase(mLasersEnemy.begin() + i--);
			continue;
			}
		mLasersEnemy[i].move(0.0f, mLasersEnemy[i].GetSpeed() * mFrameDeltaFixed.asSeconds());

		if ( mLasersEnemy[i].getGlobalBounds().intersects(mPlayer.getGlobalBounds()) )
			{
			CreateExplosionShip(mPlayer.getGlobalBounds());
			mLasersEnemy.erase(mLasersEnemy.begin() + i--);
			ChangeState(State::Dead);
			}
		}

	// check player lasers against enemies
	for(unsigned int laserIndex = 0; laserIndex < mLasersPlayer.size(); ++laserIndex)
		{
		sf::Sprite & laser {mLasersPlayer[laserIndex]};
		for ( unsigned int enemyIndex = 0; enemyIndex < mEnemies.size(); ++enemyIndex )
			{
			Enemy & enemy {mEnemies[enemyIndex]};
			const sf::FloatRect enemyRect {enemy.getGlobalBounds()};
			if ( laser.getGlobalBounds().intersects(enemyRect) )
				{
				// COLLISION
				CreateExplosionShip( enemyRect );

				mTextScoreBoard.AddScore(enemy.GetScoreValue());

				mLasersPlayer.erase( mLasersPlayer.begin() + laserIndex-- );
				mEnemies.erase( mEnemies.begin() + enemyIndex-- );
				break;
				}
			}
		}

	// check player laser against enemy laser
	for ( unsigned int playerLaserIndex = 0; playerLaserIndex < mLasersPlayer.size(); ++playerLaserIndex )
		{
		sf::Sprite & playerLaser {mLasersPlayer[playerLaserIndex]};

		for ( unsigned int enemyLaserIndex = 0; enemyLaserIndex < mLasersEnemy.size(); ++enemyLaserIndex )
			{
			sf::Sprite & enemyLaser {mLasersEnemy[enemyLaserIndex]};
			const sf::FloatRect playerLaserRect {playerLaser.getGlobalBounds()};
			if ( playerLaserRect.intersects( enemyLaser.getGlobalBounds() ) )
				{
				CreateExplosionLaser( playerLaserRect );

				mLasersEnemy.erase( mLasersEnemy.begin() + enemyLaserIndex-- );
				mLasersPlayer.erase( mLasersPlayer.begin() + playerLaserIndex-- );
				break;
				}
			}
		}
	}

void Game::UpdateExplosions()
	{
	for ( unsigned int i = 0; i < mExplosions.size(); ++i )
		{
		if ( mExplosions[i].IsAnimationOver() )
			{
			mExplosions.erase( mExplosions.begin() + i-- );
			}
		else
			{
			mExplosions[i].UpdateAnimation();
			}
		}
	}
void Game::UpdateGUI()
	{
	//gui related stuff here
	mTextTimeDisplay.Update();
	}

void Game::DrawBackgrounds(const sf::RenderStates & rstates)
	{
	mWindow.draw(mBackground1, rstates);
	mWindow.draw(mBackground2, rstates);
	}

void Game::DrawPlayer(const sf::RenderStates & rstates)
	{
	mWindow.draw(mPlayer, rstates);
	}

void Game::DrawEnemies(const sf::RenderStates & rstates)
	{
	for(auto & enemy : mEnemies)
		{
		mWindow.draw(enemy, rstates);
		}
	}

void Game::DrawLasers(const sf::RenderStates & rstates)
	{
	for(auto & laser : mLasersEnemy)
		{
		mWindow.draw(laser, rstates);
		}

	for(auto & laser : mLasersPlayer)
		{
		mWindow.draw(laser,rstates);
		}
	}

void Game::DrawExplosions(const sf::RenderStates & rstates)
	{
	for ( auto & explosion : mExplosions )
		{
		mWindow.draw(explosion,rstates);
		}
	}

void Game::DrawGUI(const sf::RenderStates & rstates)
	{
	mWindow.draw(mTextScoreBoard, rstates);
	mWindow.draw(mTextTimeDisplay, rstates);
	}

void Game::DrawInfoText(const sf::RenderStates & rstates)
	{
	mWindow.draw(mTextInfo, rstates);
	}

void Game::LoadGame()
	{
	sf::Image backgroundImage{};
	if (!backgroundImage.loadFromFile(GetTexturesFolder() + "backgroundStarsScalledCropped.png"))
		{
		throw std::runtime_error("Failed to load image.");
		}
	// create texture on graphics card from image ( copies the image from ram to graphics card memory )
	if (!mBackgroundTex1.loadFromImage(backgroundImage))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// flip the image in ram
	backgroundImage.flipVertically();

	// copy the flipped image to graphics memory as another texture.
	if (!mBackgroundTex2.loadFromImage(backgroundImage))
		{
		throw std::runtime_error("Failed to load image.");
		}

	// create and place background sprites
	mBackground1.setTexture(mBackgroundTex1);
	mBackground2.setTexture(mBackgroundTex2);
	mBackground2.setPosition(mBackground1.getPosition() + sf::Vector2f(0.0f, float(mBackground1.getGlobalBounds().height)));

	// load player texture in ResourceLoader
	texLoader.LoadResource(TextureAsset::PlayerShip, GetTexturesFolder() + "Spaceship_tut.png");
	
	// setup player sprites texture scale and position
	mPlayer.setTexture(texLoader.Get(TextureAsset::PlayerShip));
	mPlayer.setScale(sf::Vector2f{ 0.25f, 0.25f } );
	mPlayer.setPosition(GetPlayerSpawnPosition());

	texLoader.LoadResource(TextureAsset::LaserBlue, GetTexturesFolder() + "laserBlueSmaller.png");
	texLoader.LoadResource(TextureAsset::LaserRed, GetTexturesFolder() + "laserRedSmaller.png");
	texLoader.LoadResource(TextureAsset::EnemyShip, GetTexturesFolder() + "Titan.png");
	texLoader.LoadResource(TextureAsset::ExplosionShip, GetTexturesFolder() + "explosion2.png");
	texLoader.LoadResource(TextureAsset::ExplosionLaser, GetTexturesFolder() + "boom3.png");
	
	// load font
	if (!mFontGUI.loadFromFile(GetFontsFolder() + "PressStart2P.ttf"))
		{
		throw std::runtime_error("Failed to load font.");
		}

	mTextScoreBoard.setFont(mFontGUI);
	mTextScoreBoard.setPosition(sf::Vector2f{ 10.f, 10.f });
	mTextScoreBoard.setCharacterSize(25);
	mTextScoreBoard.setColor(sf::Color{ 200, 60, 60, 180 } );
	
	mTextTimeDisplay.setFont(mFontGUI);
	mTextTimeDisplay.setPosition(sf::Vector2f{ 700.f, 10.f });
	mTextTimeDisplay.setCharacterSize(25);
	mTextTimeDisplay.setColor(sf::Color{ 200, 60, 60, 180 });

	mTextInfo.setFont(mFontGUI);
	mTextInfo.setCharacterSize(25);
	mTextInfo.setColor(sf::Color{ 200, 60, 60, 180 });
	
	}

void Game::CreateEnemyLaser(const Enemy & enemy)
	{
	sf::FloatRect enemyRect {enemy.getGlobalBounds()};
	sf::Vector2f laserPos{};

	Laser enemyLaser{};
	enemyLaser.setTexture(texLoader.Get(TextureAsset::LaserRed));
	enemyLaser.setScale(0.25f, 0.25f);
	sf::FloatRect laserRect {enemyLaser.getGlobalBounds()};

	sf::Vector2f enemyHalfWidth {GetHalfWidths(enemyRect)};
	sf::Vector2f laserHalfWidth {GetHalfWidths(laserRect)};

	laserPos.x = enemyRect.left + enemyHalfWidth.x - laserHalfWidth.x;
	laserPos.y = enemyRect.top + enemyRect.height + 1.f;

	enemyLaser.setPosition( laserPos );
	mLasersEnemy.emplace_back(std::move(enemyLaser));
	mSoundManager.PlaySound(AudioEffect::LaserShot, laserPos, 80.0f, 1.2f);
	}

void Game::CreatePlayerLaser()
	{
	sf::FloatRect playerRect {mPlayer.getGlobalBounds()};
	sf::Vector2f playerHalfWidths { GetHalfWidths(playerRect) };

	Laser laser {};
	laser.setTexture(texLoader.Get(TextureAsset::LaserBlue));
	laser.setScale(0.25f,0.25f);

	sf::FloatRect laserRect { laser.getGlobalBounds() };
	sf::Vector2f laserHalfWidths { GetHalfWidths(laserRect) };

	sf::Vector2f startingLaserPos{};
	startingLaserPos.x = playerRect.left + playerHalfWidths.x - laserHalfWidths.x;
	startingLaserPos.y = playerRect.top - laserRect.height - 1.f;
	laser.setPosition( startingLaserPos );
	mLasersPlayer.emplace_back( std::move( laser ) );

	mSoundManager.PlaySound( AudioEffect::LaserShot, startingLaserPos, 80.0f, 0.85f );
	}

void Game::CreateExplosionShip(const sf::FloatRect & destroyedObjectRect)
	{
	Explosion exp{ sf::seconds(1.5f), 4, 4, 64 };

	exp.setTexture( texLoader.Get(TextureAsset::ExplosionShip ));
	exp.setTextureRect( sf::IntRect(0,0, 64, 64) );
	exp.setPosition(destroyedObjectRect.left, destroyedObjectRect.top);
	const sf::FloatRect & expRect {exp.getGlobalBounds()};

	// start with dimensions of the destroyed object, divide by the dimensions of the explosion rect
	// the result is how much to scale the explosion rect to make it the size of the destroyed object rect.
	sf::Vector2f scales{ destroyedObjectRect.width, destroyedObjectRect.height };
	scales.x /= expRect.width;
	scales.y /= expRect.height;

	exp.setScale(scales);
	
	mExplosions.emplace_back(std::move(exp));
	mSoundManager.PlaySound(AudioEffect::Explosion, sf::Vector2f(destroyedObjectRect.left, destroyedObjectRect.top), 100.0f);
	}

void Game::CreateExplosionLaser(const sf::FloatRect & destroyedLaserRect )
	{
	Explosion exp {sf::seconds(1.0f), 8, 8, 1024 / 8 };

	exp.setTexture( texLoader.Get(TextureAsset::ExplosionLaser) );
	exp.setTextureRect( sf::IntRect(0,0, 1024/8, 1024/8) );
	exp.setPosition( destroyedLaserRect.left, destroyedLaserRect.top );
	const sf::FloatRect & expRect {exp.getGlobalBounds()};

	sf::Vector2f scales( destroyedLaserRect.width, destroyedLaserRect.height );
	scales.x /= expRect.width;
	scales.y /= expRect.height;
	scales *= 2.0f;

	exp.setScale(scales);
	
	mExplosions.emplace_back(std::move(exp));
	mSoundManager.PlaySound(AudioEffect::LaserCollision, sf::Vector2f( destroyedLaserRect.left, destroyedLaserRect.top), 100.0f, 1.2f);
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

sf::Time Game::GetFrameDeltaFixed() const
	{
	return mFrameDeltaFixed;
	}

sf::Vector2f Game::GetPlayerSpawnPosition() const
	{
	const sf::Vector2f & playerHalfWidth { GetHalfWidths(mPlayer.getGlobalBounds()) };
	sf::Vector2f newPosition { mWindow.getSize() }; // convert the windows Vector2u to Vector2f
	newPosition.x *= .5f;  // half the width
	newPosition.y *= .75f; // 3/4 the height
	newPosition -= playerHalfWidth; // minus half the players width ( centering it )
	return newPosition;
	}

const Player & Game::GetPlayer() const
	{
	return mPlayer;
	}

const std::vector<Enemy> & Game::GetEnemies() const
	{
	return mEnemies;
	}

SoundManager & Game::GetSoundManager()
	{
	return mSoundManager;
	}

ScoreBoard & Game::GetScoreBoard()
	{
	return mTextScoreBoard;
	}

const std::vector<Laser> & Game::GetLasersPlayer() const
	{
	return mLasersPlayer;
	}