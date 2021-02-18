#include "map.h"

Map::Map(Config* config, Logger* main_logger) {
	// Asigna los players y rendere
	logger = main_logger;

	for (ConfigLevel* level : *(config->get_levels())) {
		levels.push_back(new Level(level, logger));
		level_amount++;
		logger->debug("Creando el nivel");
	}
	current_level = levels.at(0);
}

Map::~Map() {
	for (Level* level : levels) {
		delete level;
	}
}

bool Map::next_level() {
	if ((level_index + 1) >= level_amount) {
		status = GameStatus::END_GAME_SCORE;
		return false;
	}

	status = GameStatus::SCORE;
	time_remaining = 250;

	level_index++;
	current_level = levels.at(level_index);
	for (auto player : players) {
		player->respawn();
	}
	camera.x = 0;
	sounds.push_back(SoundType::END_LEVEL);
	contador_llegada = 0;
	for (auto player : players) {
		player->finished_level = false;
		player->puesto_llegada = 0;
	}
	return true;
}

void Map::update() {
	if (status == GameStatus::END_GAME_LOSE)
		return;

	if (status != GameStatus::PLAYING)
		time_remaining--;

	if (time_remaining > 0)
		return;
	else
		status = GameStatus::PLAYING;

	// Actualiza la posicion de los enemigos
	current_level->update();

	std::vector<Surprise*>::iterator surprises_iterator;
	std::vector<Enemy*>::iterator enemys_iterator;

	for (auto player : players) {
		// Vieja posicion para hit detection
		int old_x = player->get_x(), old_y = player->get_y();
		// Actualiza la posicion del Mario
		player->move();
		// Si colisiona pone a mario en el punto medio
		bool mario_collision = true;
		while (mario_collision) {
			mario_collision = false;
			for (Platform* platform : current_level->platforms) {
				if (platform->collision(player->get_x(), player->get_y(), player->width(), player->height()))
					mario_collision = true;

				if (platform->collision_abajo_sorpresa(player->get_x(), player->get_y(), player->width(), player->height(), player->is_jumping(), player->get_y_speed())) {
					current_level->surprises.push_back(new Surprise(platform->get_x(), platform->get_y() - platform->get_height()));
					sounds.push_back(SoundType::HIT_BLOCK);
				}
			}
			if (mario_collision) {
				player->collision(old_x, old_y);
			}
		}
		if (!test_mode) {
			for (enemys_iterator = (current_level->enemies).begin(); enemys_iterator < (current_level->enemies).end(); enemys_iterator++) {
				if ((*enemys_iterator)->collision_arriba(player->get_x(), player->get_y(), player->width(), player->height(), player->is_jumping(), player->get_y_speed())) {
					if ((*enemys_iterator)->get_type() == 1) {
						player->agregar_puntos(500);
					} else { //mata enemigo tipo 2
						player->agregar_puntos(1000);
					}
					enemys_iterator = current_level->enemies.erase(enemys_iterator);
					sounds.push_back(SoundType::STOMP_ENEMY);
				} else if (clock >= 1000 && (*enemys_iterator)->collision_costado(player->get_x(), player->get_y(), player->width(), player->height())) {
					clock = 0;
					player->restar_modo();
					sounds.push_back(SoundType::LOSE_LIFE);
				}
				if (clock < 1000)
					clock++;
			}
		}
		for (surprises_iterator = (current_level->surprises).begin(); surprises_iterator < (current_level->surprises).end(); surprises_iterator++) {
			if ((*surprises_iterator)->collision(player->get_x(), player->get_y(), player->width(), player->height())) {
				if ((*surprises_iterator)->get_type() == 1) { //agarra una moneda
					player->agregar_puntos(50);
					sounds.push_back(SoundType::PICK_UP_COIN);
				} else { //agarra un hongo
					player->subir_modo();
					sounds.push_back(SoundType::PICK_UP_MUSHROOM);
				}
				surprises_iterator = current_level->surprises.erase(surprises_iterator);
			}
		}

		// Calcula el piso y choca contra eso si es relevante
		y_floor = (LEVEL_HEIGHT - player->height());
		if (player->get_y() > y_floor && player->is_alive()) {
			player->fall_pit(test_mode);
		}
		player->bound_player(camera.x, current_level->level_width());
		//centra la camara en mario
		int new_camera_x = (player->get_x() + (player->width() / 2)) - (camera.width / 2);
		// Pide la consigna que la camera no puede volver a la izquierda
		if (camera.x < new_camera_x)
			camera.x = new_camera_x;
	}

	int amount_connected_players = 0;
	for (auto player : players) {
		if (player->is_connected() && player->is_alive())
			amount_connected_players++;
	}
	if (amount_connected_players > 0) {
		for (auto player : players) {
			if (((player->get_x() + player->width()) >= current_level->level_width()) && (player->finished_level == false)) {
				player->finished_level = true;
				contador_llegada++;
				player->puesto_llegada = contador_llegada;
			}
		}

		if (contador_llegada == amount_connected_players) {
			for (auto player : players) {
				if (player->puesto_llegada == 1) {
					player->agregar_puntos(2000);
				} else if (player->puesto_llegada == 2) {
					player->agregar_puntos(1500);
				} else if (player->puesto_llegada == 3) {
					player->agregar_puntos(1000);
				} else if (player->puesto_llegada == 4) {
					player->agregar_puntos(500);
				}
			}
			next_level();
		}

	} else {
		status = GameStatus::END_GAME_LOSE;
	}

	//mantiene camara en limites
	if (camera.x < 0)
		camera.x = 0;
	if (camera.x > current_level->level_width() - camera.width)
		camera.x = current_level->level_width() - camera.width;

}

Level* Map::get_current_level() {
	return current_level;
}

Camera* Map::get_camera() {
	return &camera;
}
