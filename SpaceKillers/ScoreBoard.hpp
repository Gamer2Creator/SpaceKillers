#pragma once
#include "SFML/Graphics/Text.hpp"
class ScoreBoard : public sf::Text
{
public:
	ScoreBoard();
	virtual ~ScoreBoard();

	int GetScore() const;
	void AddScore(const int addValue);
	void SetScore(const int value);

protected:
	int mScore;
};

