#pragma once
#include "TimeDisplay.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Explosion.hpp"
#include "ScoreBoard.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

namespace Random
{
float FloatBetween( float low = 0.0f, float high = 1.0f );
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

	void Update();
	void Draw();

	void UpdateBackground();
	void UpdatePlayer();
	void UpdateEnemies();
	void UpdateLasers();
	void UpdateExplosions();
	void UpdateGUI(); 

	void CreateEnemyLaser(Enemy & enemy);
	void CreatePlayerLaser();
	void CreateExplosionShip( const sf::FloatRect & destroyedObjectRect );
	void CreateExplosionLaser( const sf::FloatRect & destroyedLaserRect );

	static const std::string & GetTexturesFolder();
	static const std::string & GetFontsFolder();

	const sf::RenderWindow & GetWindow() const;
	sf::Time GetFrameTimeStamp() const;
	sf::Time GetFrameDelta() const;

	sf::Vector2f GetPlayerSpawnPosition() const;

	const Player & GetPlayer() const;
	ScoreBoard & GetScoreBoard();
	const std::vector<sf::Sprite> & GetLasersPlayer() const;

protected:
	sf::RenderWindow mWindow;

	sf::Time mFrameTimeStamp;
	sf::Time mFrameDelta;

	int mReturnValue;

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

	Player mPlayer;

	std::vector< Enemy > mEnemies;
	std::vector< sf::Sprite > mLasersPlayer;
	std::vector< sf::Sprite > mLasersEnemy;
	std::vector< Explosion > mExplosions;
};

extern Game * gpGame;