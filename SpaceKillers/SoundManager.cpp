#include "SoundManager.hpp"

#include "Game.hpp"

#include "SFML/Audio/Listener.hpp"

typedef std::pair<AudioEffect, std::string> AudioEffectPair;

std::vector<AudioEffectPair> gAudioEffects
	{
	{AudioEffect::LaserShot, "LaserShot.ogg"},
	{AudioEffect::Explosion, "ShipExplosion.flac"}
	};

typedef std::pair<AudioMusic, std::string> AudioMusicPair;

std::vector< AudioMusicPair > gAudioMusics
	{
	{AudioMusic::Menu, "MenuMusic.ogg"},
	{AudioMusic::Playing, "spaceGameCombat.ogg"}
	};

SoundManager::SoundManager()
	{
	LoadSounds();
	}


SoundManager::~SoundManager()
	{

	}

void SoundManager::PlaySound( AudioEffect effect, const sf::Vector2f & pos, const float volume, const float pitch )
	{
	SoundPtr pSound = std::make_unique<sf::Sound>( *(mAudioBuffers.at(effect) ) );
	pSound->setPosition(sf::Vector3f(pos.x, pos.y, 0.0f));
	pSound->setMinDistance(1000);
	pSound->setAttenuation(0);
	pSound->setPitch( pitch );
	pSound->setVolume(volume);
	pSound->play();

	mAudioPlaying.emplace_back(std::move(pSound));
	}

void SoundManager::SetMusic( AudioMusic music )
	{
	for ( auto & pIndex : gAudioMusics )
		{
		if ( pIndex.first == music )
			{
			if( !mMusic.openFromFile( Game::GetSoundsFolder() + pIndex.second ) )
				throw std::runtime_error("Failed to open music file, " + pIndex.second);
			mMusic.setVolume(25.0f);
			mMusic.setLoop(true);
			mMusic.play();
			return;
			}
		}

	mMusic.stop();
	}

void SoundManager::LoadSounds()
	{
	for ( auto & index : gAudioEffects )
		{
		SoundBufferPtr pBuffer = std::make_unique<sf::SoundBuffer> ();
		
		if ( !pBuffer->loadFromFile( Game::GetSoundsFolder() + index.second ) )
			{
			throw std::runtime_error("Failed to load audio file, " + index.second);
			}

		auto result = mAudioBuffers.emplace( std::make_pair(index.first, std::move(pBuffer) ) );
		if ( !result.second )
			{
			throw std::runtime_error("Failed to insert audio buffer, " + index.second);
			}
		}
	}

void SoundManager::Update()
	{
	sf::Vector2u size{ gpGame->GetWindow().getSize() };
	size /= 2u;
	sf::Listener::setPosition(sf::Vector3f(size.x, size.y, 0.0f));

	for ( unsigned int i = 0; i < mAudioPlaying.size(); ++i )
		{
		if ( mAudioPlaying[i]->getStatus() == sf::Sound::Status::Stopped )
			{
			mAudioPlaying.erase( mAudioPlaying.begin() + i--);
			}
		}
	}