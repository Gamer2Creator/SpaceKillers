#pragma once

#include <map>
#include <memory>
#include <string>

//sf::Texture mBackgroundTex1;
//sf::Texture mBackgroundTex2;
//
//sf::Texture mPlayerShipTex;
//sf::Texture mEnemyShipTex;
//sf::Texture mLaserBlueTex;
//sf::Texture mLaserRedTex;
//
//sf::Texture mExplosionShipTex;
//sf::Texture mExplosionLaserTex;

enum class TextureAsset
{
	Background1,
	Background2,
	PlayerShip,
	EnemyShip,
	LaserBlue,
	LaserRed,
	ExplosionShip,
	ExplosionLaser
};

template <typename Id, typename Resource> 
class ResourceLoader
{
	std::map<Id, std::unique_ptr<Resource>> resources;
public:
	ResourceLoader();
	~ResourceLoader(); 
	void LoadResource(Id id, const std::string filename);
	Resource& Get(Id id);
	
};

#include "ResourceLoader.inl"

