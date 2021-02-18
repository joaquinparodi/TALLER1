#include "platform.h"

Platform::Platform(ConfigPlatform* config, Logger* main_logger) {
	posx = config->x;
	posy = config->y;
	cantComp = config->amount;
	type = config->type;
	used = false;
}

Platform::~Platform(){}

bool Platform::collision(int x, int y, int width, int height) {
	if (x < posx + (w*cantComp) &&
		x + width > posx &&
		y < posy + h &&
		y + height > posy) {
		return true;
	}
	return false;
}

bool Platform::collision_abajo_sorpresa(int x, int y, int width, int height, bool is_jumping, int y_speed) {
	if (x < posx + w &&
		x + width > posx &&
		y < posy + h &&
		y + height > posy &&
		is_jumping &&
		y_speed < 0 &&
		type == 1 &&
		used == false) {
		used = true;
		return true;
	}
	return false;
}