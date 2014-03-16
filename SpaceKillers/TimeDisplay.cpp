#include <SFML\System\Time.hpp>
#include "Game.hpp"
TimeDisplay::TimeDisplay()
:
minutes{ 0.f },
seconds{ 0.f },
strElapsedTime{ "0" }
{
}


TimeDisplay::~TimeDisplay()
{
}

void TimeDisplay::Update()
	{
	seconds += gpGame->GetFrameDelta().asSeconds();
	
	if (((int)seconds / 60) == 1)
		{
		minutes += 1.f;
		seconds = 0;
		}
	
	strElapsedTime = std::to_string((int)minutes) + " : " + std::to_string((int)seconds);
	}

void TimeDisplay::Reset()
	{
	minutes = 0.f; 
	seconds = 0.f; 
	}