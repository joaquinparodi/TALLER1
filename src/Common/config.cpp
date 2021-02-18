#include "config.h"
#include <string>
#define DEFAULT_WIN_H 600
#define DEFAULT_WIN_W 800
#define DEFAULT_TIME 90000
#define DEFAULT_LEVEL_N 1
#define DEFAULT_C_AMOUNT 30
#define DEFAULT_C_TYPE 1
#define DEFAULT_E_AMOUNT 1
#define DEFAULT_E_TYPE 2
#define DEFAULT_P_X 100
#define DEFAULT_P_Y 200
#define DEFAULT_P_AMOUNT 5
#define DEFAULT_MAX 4
#define EMPTY_STRING 2
#define WRONG_FIELD 1
#define RIGHT_FIELD 0

using namespace tinyxml2;
using namespace std;

int Config::validate_field(const char* field) {
	bool found = false;

	if (field == nullptr)
		return EMPTY_STRING;

	std::string s = field;

	for (int i = 0; i < (int)s.size(); i++) {
		if (isalpha(s[i]) || s[i] == '-') {
			found = true;
		}
	}

	if (found)
		return WRONG_FIELD;
	return RIGHT_FIELD;
}

int Config::get_level() {
	return this->logLevel;
}

int Config::get_window_width() {
	return this->win_width;
}

int Config::get_window_height() {
	return win_height;
}

int Config::get_time() {
	return this->time;
}

int Config::get_max_players() {
	return this->max_players;
}

std::vector<ConfigLevel*>* Config::get_levels() {
	return &levels;
}

void Config::log_error(Logger* logger) {
	if (res == XML_NO_ATTRIBUTE) {
		logger->error("El archivo de configuracion no tiene niveles validos");
	} else if (res != XML_SUCCESS) {
		logger->error("Fallo la lectura del archivo de configuracion, fallo la carga de ciertos atributos");
	} else {
		std::string success = "Se cargo exitosamente el archivo de configuraci�n en ruta: ";
		success += file;
		logger->info(success.c_str());
	}
}

int log_enumeration(std::string levellog) {
	if (levellog == "DEBUG")
		return 3;

	if (levellog == "INFO")
		return 2;

	return 1;
}

void Config::load_XML(const char* path) {
	res = doc.LoadFile(path);

	if (res == XML_SUCCESS) {
		// se cargo el archivo xml correctamente

		XMLElement* configuration = doc.FirstChildElement();
		XMLElement* log = configuration->FirstChildElement();
		XMLElement* levelLog = log->FirstChildElement();
		XMLElement* windowWidth = log->NextSiblingElement()->FirstChildElement();
		XMLElement* windowHeight = log->NextSiblingElement()->FirstChildElement()->NextSiblingElement();
		XMLElement* time_remaining = log->NextSiblingElement()->LastChildElement();
		XMLElement* scene = log->NextSiblingElement()->NextSiblingElement();
		XMLElement* level = scene->FirstChildElement()->FirstChildElement();
		XMLElement* credential = log->NextSiblingElement()->NextSiblingElement()->NextSiblingElement();
		XMLElement* max_player = credential->FirstChildElement();
		XMLElement* player = credential->FirstChildElement()->NextSiblingElement()->FirstChildElement();

		if (levelLog) {
			logLevel = log_enumeration(levelLog->GetText());
		} else {
			res = XML_NO_ATTRIBUTE;
		}

		if (windowWidth && windowHeight) {
			int aux_w = validate_field(windowWidth->GetText());
			int aux_h = validate_field(windowHeight->GetText());

			if (aux_w == WRONG_FIELD)

			{
				logger->error("Tamaño de ancho de ventana incorrecto. Se setean valores por default");
				win_width = DEFAULT_WIN_W;
			} else {
				if (aux_w == EMPTY_STRING) {
					logger->error("Campo vacio en el ancho de ventana. Se setean valores por default");
					win_width = DEFAULT_WIN_W;
				} else {
					win_width = stoi(windowWidth->GetText());
				}
			}

			if (aux_h == WRONG_FIELD) {
				logger->error("Tamaño de alto de ventana incorrecto. Se setean valores por default");
				win_height = DEFAULT_WIN_H;
			} else {
				if (aux_h == EMPTY_STRING) {
					logger->error("Campo vacio en el alto de la ventana. Se setean valores por default");
					win_height = DEFAULT_WIN_H;
				} else {
					win_height = stoi(windowHeight->GetText());
				}
			}
		} else {
			res = XML_NO_ATTRIBUTE;
		}

		if (time_remaining) {
			int aux_time = validate_field(time_remaining->GetText());
			if (aux_time == WRONG_FIELD) {
				logger->error("Tiempo incorrecto. Se setea valor por default");
				time = DEFAULT_TIME;
			} else {
				if (aux_time == EMPTY_STRING) {
					logger->error("Campo vacio en el tiempo. Se setean valores por default");
					time = DEFAULT_TIME;
				} else {
					time = stoi(time_remaining->GetText());
				}
			}
		} else {
			res = XML_NO_ATTRIBUTE;
		}

		while (level) {
			XMLElement* numLevel = level->FirstChildElement();
			XMLElement* background = level->FirstChildElement()->NextSiblingElement();
			XMLElement* coins = background->NextSiblingElement();
			XMLElement* enemies = coins->NextSiblingElement();
			XMLElement* enemy = enemies->FirstChildElement();
			XMLElement* enemyType = enemy->FirstChildElement();
			XMLElement* enemyFile = enemyType->NextSiblingElement();
			XMLElement* enemyTotal = enemy->LastChildElement();
			XMLElement* platformsN = enemies->NextSiblingElement();
			XMLElement* platformN = platformsN->FirstChildElement();
			XMLElement* platformType = platformN->FirstChildElement();
			XMLElement* coodXPlatform = platformN->FirstChildElement()->NextSiblingElement();
			XMLElement* coodYPlatform = platformN->FirstChildElement()->NextSiblingElement()->NextSiblingElement();
			XMLElement* platformTotal = platformN->LastChildElement();

			if (level && numLevel && background && coins) {
				ConfigLevel* c_level = new ConfigLevel();
				int aux_level = validate_field(numLevel->GetText());
				int aux_coins = validate_field(coins->GetText());
				if (aux_level == WRONG_FIELD) {
					logger->error("Numero de nivel incorrecto. Se setea valor por default");
					c_level->number = DEFAULT_LEVEL_N;
				} else {
					if (aux_level == EMPTY_STRING) {
						logger->error("Campo vacio en el numero de nivel. Se setean valores por default");
						c_level->number = DEFAULT_LEVEL_N;
					} else {
						c_level->number = stoi(numLevel->GetText());
					}
				}

				c_level->background = background->GetText();

				if (aux_coins == WRONG_FIELD) {
					logger->error("Cantidad de monedas incorrecto. Se setea valor por default");
					c_level->coin_amount = DEFAULT_C_AMOUNT;
				} else {
					if (aux_level == EMPTY_STRING) {
						logger->error("Campo vacio en la cantidad de monedas. Se setean valores por default");
						c_level->coin_amount = DEFAULT_C_AMOUNT;
					} else {
						c_level->coin_amount = stoi(coins->GetText());
					}
				}

				levels.push_back(c_level);

				while (enemy != nullptr) {
					ConfigEnemy* enemyAux = new ConfigEnemy();
					int aux_enemy_type = validate_field(enemyType->GetText());
					int aux_enemy_amounts = validate_field(enemyTotal->GetText());
					if (aux_enemy_type == WRONG_FIELD) {
						logger->error("Tipo de enemigo incorrecto. Se setea valor por default");
						enemyAux->type = DEFAULT_E_TYPE;
					} else {
						if (aux_enemy_type == EMPTY_STRING) {
							logger->error("Campo vacio en el tipo de enemigo. Se setean valores por default");
							enemyAux->type = DEFAULT_E_TYPE;
						} else {
							enemyAux->type = stoi(enemyType->GetText());
						}
					}

					enemyAux->image = enemyFile->GetText();

					if (aux_enemy_amounts == WRONG_FIELD) {
						logger->error("Cantidad de enemigos incorrecta. Se setea valor por default");
						enemyAux->amount = DEFAULT_E_AMOUNT;
					} else {
						if (aux_enemy_amounts == EMPTY_STRING) {
							logger->error("Campo vacio en la cantidad de enemigos. Se setean valores por default");
							enemyAux->amount = DEFAULT_E_AMOUNT;
						} else {
							enemyAux->amount = stoi(enemyTotal->GetText());
						}
					}

					c_level->enemies.push_back(enemyAux);

					enemy = enemy->NextSiblingElement();

					if (enemy) {
						enemyType = enemy->FirstChildElement();
						enemyFile = enemyType->NextSiblingElement();
						enemyTotal = enemy->LastChildElement();
					}
				}

				while (platformN != nullptr) {
					ConfigPlatform* platformAux = new ConfigPlatform();
					int aux_plat_x = validate_field(coodXPlatform->GetText());
					int aux_plat_y = validate_field(coodYPlatform->GetText());
					int aux_plat_amount = validate_field(platformTotal->GetText());
					int aux_plat_type = validate_field(platformType->GetText());

					if (aux_plat_x == WRONG_FIELD) {
						logger->error("coordenada x de la posicion de la plataforma incorrecta. Se setea un valor por default");
						platformAux->x = DEFAULT_P_X;
					} else {
						if (aux_plat_x == EMPTY_STRING) {
							logger->error("Campo vacio en la posicion x de la plataforma. Se setean valores por default");
							platformAux->x = DEFAULT_P_X;
						} else {
							platformAux->x = stoi(coodXPlatform->GetText());
						}
					}
					if (aux_plat_y == WRONG_FIELD) {
						logger->error("coordenada y de la posicion de la plataforma incorrecta. Se setea un valor por default");
						platformAux->y = DEFAULT_P_Y;
					} else {
						if (aux_plat_y == EMPTY_STRING) {
							logger->error("Campo vacio en la posicion y de la plataforma. Se setean valores por default");
							platformAux->y = DEFAULT_P_Y;
						} else {
							platformAux->y = stoi(coodYPlatform->GetText());
						}
					}
					if (aux_plat_amount == WRONG_FIELD) {
						logger->error("Cantidad de plataformas incorrecta. Se setea un valor por default");
						platformAux->amount = DEFAULT_P_AMOUNT;
					} else {
						if (aux_plat_y == EMPTY_STRING) {
							logger->error("Campo vacio en la cantidad de plataformas. Se setean valores por default");
							platformAux->amount = DEFAULT_P_AMOUNT;
						} else {
							platformAux->amount = stoi(platformTotal->GetText());
						}
					}
					
					platformAux->type = stoi(platformType->GetText());

					c_level->platforms.push_back(platformAux);
					platformN = platformN->NextSiblingElement();

					if (platformN) {
						platformType = platformN->FirstChildElement();
						coodXPlatform = platformN->FirstChildElement()->NextSiblingElement();
						coodYPlatform = platformN->FirstChildElement()->NextSiblingElement()->NextSiblingElement();
						platformTotal = platformN->LastChildElement();
					}
				}

				level = level->NextSiblingElement();
			} else if (levels.size() == 0) {
				res = XML_NO_ATTRIBUTE;
			}
		}

		if (max_player) {
			int aux_max_player = validate_field(max_player->GetText());
			if (aux_max_player == WRONG_FIELD) {
				logger->error("Maxima cantidad de jugadores incorrecta. Se setea un valor por default");
				max_players = DEFAULT_MAX;
			} else {
				if (aux_max_player == EMPTY_STRING) {
					logger->error("Campo vacio en la cantidad maxima de jugadores. Se setea un valor por default");
					max_players = DEFAULT_MAX;
				} else {
					max_players = stoi(max_player->GetText());
				}
			}
		} else {
			res = XML_NO_ATTRIBUTE;
		}

		while (player != nullptr) {
			XMLElement* username = player->FirstChildElement();
			XMLElement* password = player->FirstChildElement()->NextSiblingElement();

			if (username && password) {
				ConfigUser* c_user = new ConfigUser();
				c_user->username = username->GetText();
				c_user->password = password->GetText();
				credentials.push_back(c_user);

				player = player->NextSiblingElement();
			} else if (credentials.size() == 0) {
				res = XML_NO_ATTRIBUTE;
			}
		}
	}
}

void Config::load_default() {

	logLevel = 1;
	win_width = 800;
	win_height = 600;
	time = 90000;
	max_players = 4;

	ConfigLevel* level1 = new ConfigLevel();
	ConfigLevel* level2 = new ConfigLevel();
	ConfigLevel* level3 = new ConfigLevel();
	ConfigEnemy* enemy1 = new ConfigEnemy();
	ConfigEnemy* enemy2 = new ConfigEnemy();
	level1->number = 1;
	level1->background = "assets/images/1.bmp";
	level1->coin_amount = 30;
	enemy1->type = 1;
	enemy1->image = "assets/images/goombas_0.bmp";
	enemy1->amount = 1;
	level1->enemies.push_back(enemy1);
	enemy2->type = 2;
	enemy1->image = "assets/images/koopa_0.bmp";
	enemy1->amount = 1;
	level1->enemies.push_back(enemy2);

	ConfigPlatform* platform1 = new ConfigPlatform();
	ConfigPlatform* platform2 = new ConfigPlatform();
	platform1->type = 1;
	platform1->x = 100;
	platform1->y = 200;
	platform1->amount = 5;
	level1->platforms.push_back(platform1);

	platform2->type = 2;
	platform2->x = 200;
	platform2->y = 300;
	platform2->amount = 6;
	level1->platforms.push_back(platform2);

	level2->number = 2;
	level2->coin_amount = 30;
	level2->background = "assets/images/2.bmp";

	level3->number = 3;
	level3->coin_amount = 30;
	level3->background = "assets/images/3.bmp";

	levels.push_back(level1);
	levels.push_back(level2);
	levels.push_back(level3);

	ConfigUser* user1 = new ConfigUser();
	ConfigUser* user2 = new ConfigUser();
	ConfigUser* user3 = new ConfigUser();
	ConfigUser* user4 = new ConfigUser();
	user1->username = "user1";
	user1->password = "0000";
	user2->username = "user2";
	user2->password = "0000";
	user3->username = "user3";
	user3->password = "0000";
	user4->username = "user4";
	user4->password = "0000";
	credentials.push_back(user1);
	credentials.push_back(user2);
	credentials.push_back(user3);
	credentials.push_back(user4);
}
Config::Config(const char* path, Logger* main_logger) {
	logger = main_logger;
	file.assign(path);
	try {
	load_XML(path);
	} catch (...) {
		res = XML_ERROR_PARSING_UNKNOWN;
	}
	if (res != XML_SUCCESS) {
		drop_levels();
		load_default();
	}
}

void Config::drop_levels() {
	for (ConfigLevel* level : levels) {
		for (ConfigEnemy* enemy : level->enemies) {
			delete enemy;
		}
		for (ConfigPlatform* platform : level->platforms) {
			delete platform;
		}
		delete level;
	}
}

Config::~Config() {
	drop_levels();
}
