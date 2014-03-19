#pragma once

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Text.hpp"

#include <string>

class TimeDisplay : public sf::Text
{
public:
	TimeDisplay();
	~TimeDisplay();

	void Update(); 
	sf::Time GetElapsedTime() const;
	void Reset(); 

protected:
	sf::Time mTimePassed;
};

