#include "clientenemy.h"

std::vector<std::vector<SDL_Texture*>> ClientEnemy::textures = std::vector<std::vector<SDL_Texture*>>(2);

ClientEnemy::ClientEnemy(int x, int y, int sprite_index, int type) {
    X = x;
    Y = y;
    s_i = (EnemyStatus)sprite_index;
    enemy_type = type;
}

ClientEnemy::~ClientEnemy() {
}

SDL_Texture* ClientEnemy::get_texture() {
    return ClientEnemy::textures[enemy_type - 1][(int)s_i];
}

int ClientEnemy::get_x() {
    return X;
}

int ClientEnemy::get_y() {
    return Y;
}

int ClientEnemy::width() {
    return 32;
}

int ClientEnemy::height() {
    return 32;
}

void ClientEnemy::create_sprites(SDL_Renderer* renderer) {

    //tipo 1
    ClientEnemy::textures[0].reserve((int)EnemyStatus::MAX);
    //sprites caminando a la derecha
    SDL_Surface* enemy_surface = SDL_LoadBMP("assets/images/goombas_0.bmp");
    ClientEnemy::textures[0][(int)EnemyStatus::RUN_RIGHT_1] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);
    enemy_surface = SDL_LoadBMP("assets/images/goombas_1.bmp");
    ClientEnemy::textures[0][(int)EnemyStatus::RUN_RIGHT_2] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);

    //sprites caminando a la izquierda
    enemy_surface = SDL_LoadBMP("assets/images/goombas_0.bmp");
    ClientEnemy::textures[0][(int)EnemyStatus::RUN_LEFT_1] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);
    enemy_surface = SDL_LoadBMP("assets/images/goombas_1.bmp");
    ClientEnemy::textures[0][(int)EnemyStatus::RUN_LEFT_2] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);



    //tipo 2
    ClientEnemy::textures[1].reserve((int)EnemyStatus::MAX);
    //sprites caminando a la derecha
    enemy_surface = SDL_LoadBMP("assets/images/right_koopa_0.bmp");
    ClientEnemy::textures[1][(int)EnemyStatus::RUN_RIGHT_1] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);
    enemy_surface = SDL_LoadBMP("assets/images/right_koopa_1.bmp");
    ClientEnemy::textures[1][(int)EnemyStatus::RUN_RIGHT_2] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);

    //sprites caminando a la izquierda
    enemy_surface = SDL_LoadBMP("assets/images/left_koopa_0.bmp");
    ClientEnemy::textures[1][(int)EnemyStatus::RUN_LEFT_1] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);
    enemy_surface = SDL_LoadBMP("assets/images/left_koopa_1.bmp");
    ClientEnemy::textures[1][(int)EnemyStatus::RUN_LEFT_2] = SDL_CreateTextureFromSurface(renderer, enemy_surface);
    SDL_FreeSurface(enemy_surface);

}

void ClientEnemy::delete_sprites() {
    for (auto texturess : textures) {
        for (auto texture : texturess) {
            std::cerr << "deleted enemy\n";
            SDL_DestroyTexture(texture);
        }
    }
}

