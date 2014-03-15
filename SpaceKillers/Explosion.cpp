#include "Explosion.hpp"

#include "Game.hpp"


Explosion::Explosion(const sf::Time timeLength,
	int framesCountWidth,
	int framesCountHeight,
	int frameWidthHeightPixels)
	:
	mAnimationLength(timeLength),
	mAnimationStart(gpGame->GetFrameTimeStamp()),
	mFramesCountWidth(framesCountWidth),
	mFramesCountHeight(framesCountHeight),
	mFrameWidthHeightPixels(frameWidthHeightPixels)
	{

	}


Explosion::~Explosion()
	{

	}

void Explosion::UpdateAnimation()
	{
	sf::Time animationPosition = gpGame->GetFrameTimeStamp() - mAnimationStart;

	const long long posInt = animationPosition.asMilliseconds();
	const long long maxInt = mAnimationLength.asMilliseconds();

	const long long timePerFrame = maxInt / (mFramesCountWidth * mFramesCountHeight);
	
	const long long frame = posInt / timePerFrame;

	sf::IntRect rect;
	rect.height = rect.width = mFrameWidthHeightPixels;
	rect.left = frame % mFramesCountWidth * rect.width;
	rect.top = frame / mFramesCountWidth * rect.height;

	setTextureRect(rect);
	}

bool Explosion::IsAnimationOver() const
	{
	if( mAnimationStart + mAnimationLength <= gpGame->GetFrameTimeStamp() )
		{
		return true;
		}
	else
		return false;
	}