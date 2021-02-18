#include "clientlevel.h"

std::unordered_map<int, SDL_Texture*> ClientLevel::textures = std::unordered_map<int, SDL_Texture*>();

ClientLevel::ClientLevel(int num) {
    number = num;
    SDL_QueryTexture(get_texture(), NULL, NULL, &w, NULL);
}

ClientLevel::~ClientLevel() {
}

SDL_Texture* ClientLevel::get_texture() {
	return textures[number];
}

int ClientLevel::width() {
    return w;
}

int ClientLevel::get_number() {
    return number;
}

void ClientLevel::create_sprites(Config* config, SDL_Renderer* rend) {
    std::vector<ConfigLevel*>::iterator c_level = config->get_levels()->begin();
    while (c_level != config->get_levels()->end()){
        SDL_Surface* surface = SDL_LoadBMP((*c_level)->background.c_str());
        textures[(*c_level)->number] = SDL_CreateTextureFromSurface(rend, surface);
        SDL_FreeSurface(surface);
        c_level = std::next(c_level);
    }
}

void ClientLevel::delete_sprites() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
}
