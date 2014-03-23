#pragma once

#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/System/Vector2.hpp"

#include <map>
#include <vector>
#include <memory>

enum class AudioEffect
	{
	LaserShot,
	Explosion,
	Turbo
	};

enum class AudioMusic
	{
	Menu,
	Playing,
	Dead
	};

typedef std::unique_ptr<sf::Sound> SoundPtr;
typedef std::unique_ptr<sf::SoundBuffer> SoundBufferPtr;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Update();

	void PlaySound( AudioEffect effect, const sf::Vector2f & pos, const float volume = 100.0f, const float pitch = 1.0f );

	void SetMusic( AudioMusic music );

protected:
	void LoadSounds();

	sf::Music mMusic;

	std::map< AudioEffect, SoundBufferPtr > mAudioBuffers;
	std::vector< SoundPtr > mAudioPlaying;
};

