#pragma once
#include "../Common/logger.h"
#include "../Server/surprise.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class ClientSurprise {

public:
	ClientSurprise(int x, int y, int s_type);
	~ClientSurprise();

	SDL_Texture* get_texture();
	int get_x();
	int get_y();
	int width();
	int height();

	static void create_sprites(SDL_Renderer* renderer);
	static void delete_sprites();
	static std::vector<SDL_Texture*> textures;

private:
	int type;
	int X; // Sprite Index
	int Y; // Sprite Index
	Logger* log;
	SDL_Renderer* renderer;

	SDL_Surface* image_error();
};

