#include "scoreboard.h"

Scoreboard::Scoreboard()
{
}

void Scoreboard::addScore(std::string name, int score)
{
	bool found = false;
	for (auto player_score : board)
	{
		if (player_score->name == name)
		{
			player_score->totalScore = score;
			found = true;
		}
	}
	if (!found)
	{
		auto new_score = new player_name_score();
		new_score->name = name;
		new_score->totalScore = score;
		board.push_back(new_score);
	}
}

void Scoreboard::updateScore()
{

	for (auto score : board)
	{
		int previous_score = 0;

		if (score->parcialScore.size() > 0)
		{
			for (auto parcial : score->parcialScore)
			{
				previous_score += parcial;
			}
		}

		score->parcialScore.push_back(score->totalScore - previous_score);
	}
}
std::string Scoreboard::winner()
{

	int mayor = 0;
	std::string winner;
	for (auto score : board)
	{
		if (mayor < score->totalScore)
		{
			mayor = score->totalScore;
			winner=score->name;
		}
	}
	return winner;
}

Scoreboard::~Scoreboard()
{
	for (auto score : board)
	{
		delete score;
	}
}
