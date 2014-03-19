#include <SFML\System\Time.hpp>
#include "Game.hpp"
TimeDisplay::TimeDisplay()
	:
	sf::Text{},
	mTimePassed{sf::milliseconds(0)}
	{
	}


TimeDisplay::~TimeDisplay()
	{
	}

void TimeDisplay::Update()
	{
	mTimePassed += gpGame->GetFrameDeltaFixed();

	int milliseconds = mTimePassed.asMilliseconds();

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
	}

sf::Time TimeDisplay::GetElapsedTime() const
	{
	return mTimePassed;
	}

void TimeDisplay::Reset()
	{
	mTimePassed = sf::milliseconds(0);
	}

