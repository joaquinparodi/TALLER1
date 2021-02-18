#include "clientplayer.h"

std::vector<std::vector<SDL_Texture*>> ClientPlayer::textures = std::vector<std::vector<SDL_Texture*>>(5);

ClientPlayer::ClientPlayer(int x, int y, int h, int sprite_index, int p_number, int vidas, int puntos_actuales, char* username) {
    vidas_restantes = vidas;
    puntos = puntos_actuales;
    X = x;
    Y = y;
    H = h;
    s_i = (PlayerStatus)sprite_index;
    player_number = p_number;
    User.assign(username);
}

ClientPlayer::~ClientPlayer() {
}

SDL_Texture* ClientPlayer::get_texture() {
    return ClientPlayer::textures[player_number][(int)s_i];
}

int ClientPlayer::get_vidas() {
    return vidas_restantes;
}

int ClientPlayer::get_puntos() {
    return puntos;
}

int ClientPlayer::get_x() {
    return X;
}

int ClientPlayer::get_y() {
    return Y;
}

int ClientPlayer::width() {
    return 32;
}

int ClientPlayer::height() {
    return H;
}


void ClientPlayer::create_sprites(SDL_Renderer* renderer, Logger* main_logger) {
    std::string filename;
    for (int i = 0; i < 5; i++) {
        std::string filepath = std::string("assets/images/mario/mario " + std::to_string(i));

        ClientPlayer::textures[i].reserve((int)PlayerStatus::MAX);

        //sprite principal
        filename = filepath + std::string("/mario.bmp");
        SDL_Surface* player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::STOP] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_jump.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::JUMP] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        //sprites caminando a la derecha
        filename = filepath + std::string("/mario_right_1.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_RIGHT_1] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_right_2.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_RIGHT_2] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_right_3.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_RIGHT_3] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        //sprites caminando a la derecha
        filename = filepath + std::string("/mario_left_1.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_LEFT_1] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_left_2.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_LEFT_2] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_left_3.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::RUN_LEFT_3] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);

        filename = filepath + std::string("/mario_squat.bmp");
        player_surface = SDL_LoadBMP(filename.c_str());
        ClientPlayer::textures[i][(int)PlayerStatus::BEND] = SDL_CreateTextureFromSurface(renderer, player_surface);
        SDL_FreeSurface(player_surface);
    }
}

void ClientPlayer::delete_sprites() {
    for (auto texturess : textures) {
        for (auto texture : texturess) {
            SDL_DestroyTexture(texture);
        }
    }
}

SDL_Surface* ClientPlayer::image_error(Logger* main_logger) {
    main_logger->error("No se ha podido cargar una imagen de mario. Se carga una por defecto.");
    return SDL_LoadBMP("assets/images/not_found.bmp");
}
