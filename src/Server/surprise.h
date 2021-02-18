#pragma once
#include "../Common/config.h"
#include "../Common/logger.h"

class Surprise {
private:
    int posx;
    int w = 32;
    int posy;
    int h = 32;
    int type; // 1 -> coin , 2 -> hongo

public:
    Surprise(int X, int Y);
    ~Surprise();
    int get_type();
    int get_x();
    // Getter de la posicion Y
    int get_y();
    // Ancho del sprite
    int width();
    // Alto del sprite
    int height();

    bool collision(int x, int y, int width, int height);
};