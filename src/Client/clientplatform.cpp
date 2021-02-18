#include "clientplatform.h"

std::vector<SDL_Texture*> ClientPlatform::textures = std::vector<SDL_Texture*>(3);


ClientPlatform::ClientPlatform(int x, int y, int p_amount, int p_type) {
    X = x;
    Y = y;
    amount = p_amount;
    type = p_type;
}

ClientPlatform::~ClientPlatform() {
}

SDL_Texture* ClientPlatform::get_texture() {
    return ClientPlatform::textures[type - 1];
}

int ClientPlatform::get_x() {
    return X;
}

int ClientPlatform::get_y() {
    return Y;
}

int ClientPlatform::get_amount() {
    return amount;
}

int ClientPlatform::width() {
    return 32;
}

int ClientPlatform::height() {
    return 32;
}

void ClientPlatform::create_sprites(SDL_Renderer* renderer) {

    ClientPlatform::textures.reserve(4);
    //tipo 1
    SDL_Surface* platform_surface = SDL_LoadBMP("assets/images/sorpresa.bmp");
    ClientPlatform::textures[0] = SDL_CreateTextureFromSurface(renderer, platform_surface);
    SDL_FreeSurface(platform_surface);

    //tipo 2
    platform_surface = SDL_LoadBMP("assets/images/ladrillo.bmp");
    ClientPlatform::textures[1] = SDL_CreateTextureFromSurface(renderer, platform_surface);
    SDL_FreeSurface(platform_surface);

    //tipo 3
    platform_surface = SDL_LoadBMP("assets/images/tunel.bmp");
    ClientPlatform::textures[2] = SDL_CreateTextureFromSurface(renderer, platform_surface);
    SDL_FreeSurface(platform_surface);

    ClientPlatform::textures[3] = NULL;
}

void ClientPlatform::delete_sprites() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture);
    }
}