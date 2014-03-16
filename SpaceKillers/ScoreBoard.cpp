#include "ScoreBoard.hpp"


ScoreBoard::ScoreBoard()
	:
	sf::Text{},
	mScore{0}
	{
	setString('0');
	}


ScoreBoard::~ScoreBoard()
	{
	}

int ScoreBoard::GetScore() const
	{
	return mScore;
	}

void ScoreBoard::AddScore(const int addScore)
	{
	mScore += addScore;

	mScore = std::max(mScore, 0);

	setString( std::to_string(mScore) );
	}

void ScoreBoard::SetScore(const int setScore)
	{
	mScore = setScore;

	mScore = std::max(mScore, 0);

	setString( std::to_string(mScore) );
	}