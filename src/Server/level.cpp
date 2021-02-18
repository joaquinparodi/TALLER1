#include "level.h"
#include <iostream>
#include <fstream>

Level::Level(ConfigLevel* config, Logger* logger) {
    number = config->number;
    std::ifstream background;
    background.open(config->background, std::ios::in | std::ios::binary);
    background.seekg(18);
    char width[2];
    background.read(width, 2);
    // Magic
    w = ((unsigned char)width[0] & 0xFF) + (((unsigned char)width[1] & 0xFF) << 8);
    number = config->number;

    //Crea los enemigos
    for (ConfigEnemy* c_enemy : config->enemies) {
        for (int i = 0; i < c_enemy->amount; i++) {
            enemies.push_back(new Enemy(c_enemy, level_width(), logger));
        }
    }    

    //Crea las Plataformas
    for (ConfigPlatform* c_platform : config->platforms) {
        for (int i = 0; i < c_platform->amount; i++) {
            platforms.push_back(new Platform(c_platform, logger));
        }
    }
}

Level::~Level() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    for (Platform* platform : platforms) {
        delete platform;
    }
    for (Surprise* surprise : surprises) {
        delete surprise;
    }
}

void Level::update() {
    for (Enemy* enemy : enemies) {
        enemy->move();
        enemy->bounce_wall(0, level_width());
    }
}
