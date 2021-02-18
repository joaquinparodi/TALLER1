#include "surprise.h"
#include "map.h"

Surprise::Surprise(int X, int Y){
    posx = X;
    posy = Y;
	type = 1 + rand() % 2;
}

Surprise::~Surprise(){
}

int Surprise::get_type() {
    return type;
}

int Surprise::get_x() {
    return posx;
}

int Surprise::get_y() {
    return posy;
}

int Surprise::width() {
    return w;
}

int Surprise::height() {
    return h;
}

bool Surprise::collision(int x, int y, int width, int height) {
	if (x < posx + w &&
		x + width > posx &&
		y < posy + h &&
		y + height > posy) {
		return true;
	}
	return false;
}


