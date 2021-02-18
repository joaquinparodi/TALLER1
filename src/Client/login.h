#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "../Common/logger.h"
#include "../Common/socket.h"

// El mapa es el responsable de organizor todos los actores en el mapas
// Y de renderizarlo
class Login {
private:
	float render_factor = 1;
	
	Socket* sock;

	SDL_Renderer* renderer = NULL;
	TTF_Font* font;
	void print_username(std::string username);
	void print_password(std::string password);
	void print_response(std::string response);
public:
	Login(SDL_Renderer* rend, TTF_Font* f, float r_f, Socket* sock);

	// Logged In Data
	std::string input = "";
	std::string input_p = "";
	std::string input_password = "";
	std::string current_input = "username";
	std::string response_s = "";
	bool data_filled = false;

	bool logged_in = false;

	//render
	bool input_login(SDL_Event* event);
	bool in_bound_user(int x, int y);
	bool in_bound_pass(int x, int y);
	void render();
	void login();
};