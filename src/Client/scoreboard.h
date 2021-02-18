#include "mapstate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

using namespace std;

typedef struct player_name_score {
	std::string name;
	int totalScore;
	std::vector<int> parcialScore;

} player_name_score_t;

class Scoreboard {
public:
	Scoreboard();
	~Scoreboard();

	std::vector<player_name_score_t*> board;
	void addScore(std::string name, int score);
	void updateScore();
	std::string winner();
};
