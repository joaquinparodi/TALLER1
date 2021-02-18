#pragma once
#include <vector>
#include <string>
#include "logger.h"
#include "TinyXML2/tinyxml2.h"

using namespace tinyxml2;

class ConfigEnemy {
public:
	int type;
	std::string image;
	int amount;
};

class ConfigPlatform {
public:
	int type;
	int x;
	int y;
	int amount;
};

class ConfigLevel {
public:
	int number;
	std::string background;
	int coin_amount;
	std::vector<ConfigEnemy*> enemies;
	std::vector<ConfigPlatform*> platforms;
};
class ConfigUser {
public:
	std::string username;
	std::string password;

};

class Config {
private:
	void load_default();
	void load_XML(const char* path);
	XMLDocument doc;
	XMLError res;

	int logLevel;
	int win_width;
	int win_height;
	int time;
	int max_players;
	std::vector<ConfigLevel*> levels;
	std::string file;


public:
	Config(const char* path, Logger* logger);
	void drop_levels();
	~Config();

	int get_level();
	int get_window_width();
	int get_window_height();
	int get_time();
	int get_max_players();
	std::vector<ConfigLevel*>* get_levels();
	std::vector<ConfigUser*> credentials;

	void log_error(Logger* logger);
	int validate_field(const char* field);
	Logger* logger;
};
