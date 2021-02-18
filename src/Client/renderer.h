#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "mapstate.h"
#include "sender.h"
#include "receiver.h"
#include "login.h"
#include "scoreboard.h"
#include "sound.h"

enum class RenderStatus : int {
	LOGIN,
	WAITING,
	PLAYING,
	SCORE,
	ENG_GAME_SCORE,
	ENG_GAME_LOST,
	DISCONNECTED
};

//#include"server_number_queue.h"
class Renderer {
public:
	Renderer(Config* config, Logger* logger);
	~Renderer();

	void run(Sender* sender, Receiver* receiver, Socket* sock);
private:
	// SDL Stuff
	SDL_Renderer* renderer;
	SDL_Texture* background_texture;
	SDL_Texture* wait_texture;
	SDL_Texture* disc_texture;
	SDL_Window* win;
	SDL_Rect* camera;
	TTF_Font* font;
	SDL_Color white;

	//Scoreboard
	Scoreboard scoreboard;

	// Sound
	Sound* sound_subsystem;

	// Thread Organization
	Sender* sen;
	Receiver* rec;

	// Code Organization
	RenderStatus status = RenderStatus::LOGIN;
	MapState* map;
	Login* log_in;

	// Login
	void render_login();
	void login();

	// Waiting
	bool input_wait(SDL_Event* event);
	void render_wait();

	// Playing
	bool input_map(SDL_Event* event);
	void render_map();

	// Disconnected
	bool input_disc(SDL_Event* event);
	void render_disc();

	// Score
	void render_score();
	void render_end_game_score();

	void render_lose();

	//tiempo del juego en milisegundos
    int level_time;
    int current_time = 0;
    int current_world = 1;
    int time;


	//Funciones de la impresion de texto
	void print_fps(int frame_time);
	void print_time_remaining(int passed_time);
	void print_level_indicator(int number_of_level);
	void print_server_status(const char* status);
	void print_scoreboard_level();
	void print_scoreboard_final();
	void print_msg(std::string msg, int x, int y, int w, int h);
};
