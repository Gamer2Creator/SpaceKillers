#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Laser.hpp"
#include "Explosion.hpp"
#include "ScoreBoard.hpp"
#include "TimeDisplay.hpp"
#include "SoundManager.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <limits>

enum class State
	{
	Null = 0,
	MainMenu,
	Playing,
	Paused,
	Dead,
	};

namespace Random
{
float FloatBetween( float low = 0.0f, float high = 1.0f );
long long LongBetween( long long low = 0, long long high = std::numeric_limits<long long>::max() );
} 

class Game
{
public:
	Game();
	~Game();

	void LoadGame();

	void MainLoop();
	int GetReturnValue() const;

	void ResetGame();
	
	// state code
	void UpdateState();
	void DrawState();
	void ChangeState( State state );

	// Main Menu state
	void UpdateStateMainMenu();
	void DrawStateMainMenu();

	// Playing state
	void UpdateStatePlaying();
	void DrawStatePlaying();

	// Dead state
	void UpdateStateDead();
	void DrawStateDead();

	void UpdateBackground();
	void UpdatePlayer();
	void UpdateEnemies();
	void UpdateLasers();
	void UpdateExplosions();
	void UpdateGUI();

	void DrawBackgrounds(const sf::RenderStates & rstates);
	void DrawPlayer(const sf::RenderStates & rstates);
	void DrawEnemies(const sf::RenderStates & rstates);
	void DrawLasers(const sf::RenderStates & rstates);
	void DrawExplosions(const sf::RenderStates & rstates);
	void DrawGUI(const sf::RenderStates & rstates);
	void DrawInfoText(const sf::RenderStates & rstates);

	void CreateEnemyLaser(const Enemy & enemy);
	void CreatePlayerLaser();
	void CreateExplosionShip( const sf::FloatRect & destroyedObjectRect );
	void CreateExplosionLaser( const sf::FloatRect & destroyedLaserRect );

	static const std::string & GetTexturesFolder();
	static const std::string & GetFontsFolder();
	static const std::string & GetSoundsFolder();

	const sf::RenderWindow & GetWindow() const;
	sf::Time GetFrameTimeStamp() const;
	sf::Time GetFrameDelta() const;
	sf::Time GetFrameDeltaFixed() const;

	sf::Vector2f GetPlayerSpawnPosition() const;

	const Player & GetPlayer() const;
	SoundManager & GetSoundManager();
	ScoreBoard & GetScoreBoard();
	const std::vector<Laser> & GetLasersPlayer() const;

protected:
	sf::RenderWindow mWindow;

	State mCurrentState;

	sf::Time mFrameTimeStamp;
	sf::Time mFrameDelta;
	sf::Time mFrameDeltaFixed;

	int mReturnValue;
	float mBackgroundSpeed;

	// textures
	sf::Texture mBackgroundTex1;
	sf::Texture mBackgroundTex2;

	sf::Texture mPlayerShipTex;
	sf::Texture mEnemyShipTex;
	sf::Texture mLaserBlueTex;
	sf::Texture mLaserRedTex;

	sf::Texture mExplosionShipTex;
	sf::Texture mExplosionLaserTex;

	// fonts
	sf::Font mFontGUI;

	// sprites
	sf::Sprite mBackground1;
	sf::Sprite mBackground2;

	// gui text
	ScoreBoard mTextScoreBoard;
	TimeDisplay mTextTimeDisplay;
	sf::Text mTextInfo;

	Player mPlayer;

	std::vector< Enemy > mEnemies;
	std::vector< Laser > mLasersPlayer;
	std::vector< Laser > mLasersEnemy;
	std::vector< Explosion > mExplosions;

	SoundManager mSoundManager;
};

extern Game * gpGame;