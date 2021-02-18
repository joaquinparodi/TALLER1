#pragma once

#include "../Common/config.h"
#include "../Common/logger.h"
#include <iostream>

class Platform {
private:
	int posx, posy;
	int cantComp;
	int w = 32, h = 32;
	int type;
	Logger* logger;
	bool used;

public:

	Platform(ConfigPlatform* config, Logger* main_logger);
	~Platform();
	int get_x() { return posx; }
	int get_y() { return posy; }
	int get_width() { return w; }
	int get_type() { return type; }
	int get_height() { return h; }
	int get_amount() { return cantComp; }
	bool collision(int x, int y, int width, int height);
	bool collision_abajo_sorpresa(int x, int y, int width, int height, bool is_jumping, int y_speed);
	
};
