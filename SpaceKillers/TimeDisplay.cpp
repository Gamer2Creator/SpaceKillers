#include <SFML\System\Time.hpp>
#include "Game.hpp"
TimeDisplay::TimeDisplay()
	:
	sf::Text{},
	mStartTime{ }
	{
	}


TimeDisplay::~TimeDisplay()
	{
	}

void TimeDisplay::Update()
	{
	sf::Time elapsedTime { GetElapsedTime() };

	int milliseconds = elapsedTime.asMilliseconds();

	static const int minuteMillis = 60 * 1000;

	int minutes = milliseconds / minuteMillis;
	int seconds = milliseconds % minuteMillis / 1000;

	std::string elapsedTimeString{};
	elapsedTimeString.reserve(10);
	
	elapsedTimeString = std::to_string(minutes) + ":";
	if ( seconds < 10 )
		elapsedTimeString.push_back('0');
	elapsedTimeString += std::to_string(seconds);

	setString( elapsedTimeString );
	/*
	seconds += gpGame->GetFrameDelta().asSeconds();
	
	if (((int)seconds / 60) == 1)
		{
		minutes += 1.f;
		seconds = 0;
		}
	
	strElapsedTime = std::to_string((int)minutes) + " : " + std::to_string((int)seconds);
	*/
	}

sf::Time TimeDisplay::GetElapsedTime() const
	{
	return gpGame->GetFrameTimeStamp() - mStartTime;
	}
void TimeDisplay::Reset()
	{
	mStartTime = gpGame->GetFrameTimeStamp();
	}

