#pragma once

#include "../Common/logger.h"
#include "../Server/player.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class ClientPlayer {
public:
	ClientPlayer(int x, int y, int h, int sprite_index, int p_number, int vidas, int puntos_actuales, char* username);
	~ClientPlayer();

	SDL_Texture* get_texture();
	int get_vidas();
	int get_puntos();
	int get_x();
	int get_y();
	int width();
	int height();
	std::string get_name() { return User; }


	static void create_sprites(SDL_Renderer* renderer, Logger* main_logger);
	static void delete_sprites();
	static std::vector <std::vector<SDL_Texture*>> textures;
private:

	int vidas_restantes=3;
	int puntos = 0;
	int X; // Sprite Index
	int Y; // Sprite Index
	PlayerStatus s_i; // Sprite Index
	std::string User;
	int H;

	static SDL_Surface* image_error(Logger* main_logger);
	int player_number;
};
