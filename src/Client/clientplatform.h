#pragma once
#include "../Common/logger.h"
#include "../Server/platform.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class ClientPlatform {

public:
	ClientPlatform(int x, int y, int amount, int type);
	~ClientPlatform();

	SDL_Texture* get_texture();
	int get_x();
	int get_y();
	int width();
	int height();
	int get_amount();

	static void create_sprites(SDL_Renderer* renderer);
	static void delete_sprites();
	static std::vector<SDL_Texture*> textures;

private:
	int X; // Sprite Index
	int Y; // Sprite Index
	int amount;
	int type;
	Logger* log;
	SDL_Renderer* renderer;
};