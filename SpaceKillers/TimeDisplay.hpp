#pragma once
#include <string>
class TimeDisplay
{
	float seconds;
	float minutes; 
	std::string strElapsedTime;
public:
	TimeDisplay();
	~TimeDisplay();
	void Update(); 
	inline std::string& getElapsedTime() { return strElapsedTime; }
	void Reset(); 
};

