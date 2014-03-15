#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

class Explosion : public sf::Sprite
{
public:
	Explosion(const sf::Time timeLength,
			  int framesCountWidth,
			  int framesCountHeight,
			  int frameWidthHeightPixels);
	virtual ~Explosion();

	virtual void UpdateAnimation();

	bool IsAnimationOver() const;

protected:
	sf::Time mAnimationLength;
	sf::Time mAnimationStart;

	int mFrameCurrent;

	int mFramesCountWidth;
	int mFramesCountHeight;

	int mFrameWidthHeightPixels;
};

