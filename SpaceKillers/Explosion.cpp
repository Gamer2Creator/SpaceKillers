#include "Explosion.hpp"

#include "Game.hpp"


Explosion::Explosion(const sf::Time timeLength,
	int framesCountWidth,
	int framesCountHeight,
	int frameWidthHeightPixels)
	:
	mAnimationLength(timeLength),
	mAnimationStart(gpGame->GetFrameTimeStamp()),
	mFrameCurrent(0),
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

	const int posInt = animationPosition.asMilliseconds();
	const int maxInt = mAnimationLength.asMilliseconds();

	const int timePerFrame = maxInt / (mFramesCountWidth * mFramesCountHeight);
	
	const int frame = posInt / timePerFrame;

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