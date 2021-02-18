#pragma once
#include "../Common/logger.h"
#include "../Server/enemy.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class ClientEnemy {

public:
	ClientEnemy(int x, int y, int sprite_index, int type);
	~ClientEnemy();

	SDL_Texture* get_texture();
	int get_x();
	int get_y();
	int width();
	int height();

	static void create_sprites(SDL_Renderer* renderer);
	static void delete_sprites();
	static std::vector <std::vector<SDL_Texture*>> textures;

private:
	int X; // Sprite Index
	int Y; // Sprite Index
    int enemy_type;
	EnemyStatus s_i; // Sprite Index
};
