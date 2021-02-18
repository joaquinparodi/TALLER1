#include "clientsurprise.h"

std::vector<SDL_Texture*> ClientSurprise::textures = std::vector<SDL_Texture*>(2);

ClientSurprise::ClientSurprise(int x, int y, int s_type) {
    X = x;
    Y = y;
    type = s_type;
}

ClientSurprise::~ClientSurprise() {
}

SDL_Texture* ClientSurprise::get_texture() {
    return ClientSurprise::textures[type-1];
}

int ClientSurprise::get_x() {
    return X;
}

int ClientSurprise::get_y() {
    return Y;
}

int ClientSurprise::width() {
    return 32;
}

int ClientSurprise::height() {
    return 32;
}


void ClientSurprise::create_sprites(SDL_Renderer* renderer) {
    ClientSurprise::textures.reserve(2);
    //tipo 1 coin
    SDL_Surface* surface = SDL_LoadBMP("assets/images/coin.bmp");
    ClientSurprise::textures[0] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    //tipo 2 hongo
    surface = SDL_LoadBMP("assets/images/hongo.bmp");
    ClientSurprise::textures[1] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void ClientSurprise::delete_sprites() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture);
    }
}