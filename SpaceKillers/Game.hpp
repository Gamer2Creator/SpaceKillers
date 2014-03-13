#pragma once

#include "Player.hpp"
#include "Enemy.hpp"

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

	void Update();
	void Draw();

	void UpdateBackground();
	void UpdatePlayer();
	void UpdateEnemies();
	void UpdateLasers();

	void CreateEnemyLaser(Enemy & enemy);
	void CreatePlayerLaser();

	static const std::string & GetTexturesFolder();

	const sf::RenderWindow & GetWindow() const;
	sf::Time GetFrameTimeStamp() const;
	sf::Time GetFrameDelta() const;

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

	// sprites
	sf::Sprite mBackground1;
	sf::Sprite mBackground2;

	Player mPlayer;

	std::vector< Enemy > mEnemies;
	std::vector< sf::Sprite > mLasersPlayer;
	std::vector< sf::Sprite > mLasersEnemy;
};

extern Game * gpGame;