#pragma once
#include "../Common/config.h"
#include "../Common/logger.h"


enum struct EnemyStatus {
    RUN_LEFT_1,
    RUN_LEFT_2,
    RUN_RIGHT_1,
    RUN_RIGHT_2,
    MAX,
};

// Posicion, Velocida, Aceleracion
// datos de juego
class Enemy {
private:
    int x = 50;
    int w = 32;
    int x_speed = 2;
    int y = 385;
    int h = 32;
    bool right = true;
    int enemy_type;
    Logger* log = NULL;

    const int sprite_limit = 1;
    int sprite_clock = 0;
    EnemyStatus sprite_index = EnemyStatus::RUN_LEFT_1;

public:
    Enemy(ConfigEnemy* c_enemy, int right_wall, Logger* logger);
    ~Enemy();
    int get_x();
    // Getter de la posicion Y
    int get_y();
    // Ancho del sprite
    int width();
    // Alto del sprite
    int height();
    // Get Type
    int get_type();
    // Get Type
    EnemyStatus get_sprite_index();
    // Funcion que se ejecuta por frame para actualizar la posicion
    void move();
    void bounce_wall(int left_wall, int right_wall);
    bool collision_arriba(int pos_x, int pos_y, int width, int height, bool is_jumping, int y_speed);
    bool collision_costado(int pos_x, int pos_y, int width, int height);
};