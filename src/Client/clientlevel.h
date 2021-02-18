#pragma once

#include <vector>
#include "clientenemy.h"
#include "clientsurprise.h"
#include "clientplatform.h"
#include "../Common/config.h"
#include "../Common/gamestate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>

class ClientLevel {
private:
	int w;
	static std::unordered_map<int,SDL_Texture*> textures;
	int number;
public:
	ClientLevel(int number);
	~ClientLevel();
	SDL_Texture* get_texture();
	int width();
	int get_number();
	static void create_sprites(Config* config, SDL_Renderer* rend);
	static void delete_sprites();
};
