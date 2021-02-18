#pragma once
#include <vector>
#include "enemy.h"
#include "platform.h"
#include "surprise.h"
#include "../Common/logger.h"

class Level {
private:
    int w, number;
    
public:
    Level(ConfigLevel* config, Logger* logger);
    ~Level();

    std::vector<Enemy*> enemies;
    std::vector<Platform*> platforms;
    std::vector<Surprise*> surprises;
    void update();
    int level_width() { return w; }
    int get_number() { return number; }
    std::vector<Enemy*> get_enemies() { return enemies; }
    std::vector<Platform*> get_platforms() { return platforms; }
    std::vector<Surprise*> get_surprises() { return surprises; }
};
