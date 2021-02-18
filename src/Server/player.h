#pragma once
#include "../Common/logger.h"

enum struct PlayerStatus : int {
	STOP,
	RUN_LEFT_1,
	RUN_LEFT_2,
	RUN_LEFT_3,
	RUN_RIGHT_1,
	RUN_RIGHT_2,
	RUN_RIGHT_3,
	BEND,
	JUMP,
	MAX,
};

enum struct PlayerMode : int {
	SMALL,
	BIG
};

static const int PLAYER_VEL = 7;

// Guarda los datos del mario
// Posicion, Velocida, Aceleracion
// Sprites y datos de juego
class Player {
private:
	PlayerMode modo; // 1 -> chiquito , 2 -> grande.
	int cantidad_vidas;
	int x,
		y,
		last_x = 0,
		last_y = 0;
	int w;
	int h, h_short;
	int y_speed;
	int x_speed;
	int y_accel;
	int puntos;
	Logger* log = NULL;

	bool left = false;
	bool right = false;
	bool is_bending = false;
	bool jumping = false;
	bool connected = true;
	bool alive = true;

	PlayerStatus sprite_index = PlayerStatus::STOP;
	const int sprite_limit = 2;
	int sprite_counter = 0;
	int sprite_clock = 0;
	int player_number = 1;


public:
	Player(Logger* logger, std::string user, int number_of_player);
	~Player();
	int get_y_speed();
	void subir_modo();
	void restar_modo();
	void restar_vida();
	bool is_connected();
	bool is_alive();
	// funcion que pone al player al inicio del nivel
	void respawn();
	// Funcion que se ejecuta por frame para actualizar la posicion
	void move();
	// Getter de la posicion X
	int get_x();
	// Getter de la posicion Y
	int get_y();
	// Ancho del sprite
	int width();
	// Alto del sprite
	int height();
	// Consigue en que estado visual esta mario
	int get_sprite_index();
	//Salta
	void jump();
	void hit_floor(int y_floor);
	void fall_pit(bool test_mode);
	// Se agacha
	void bend(bool bending);
	// Le indica a mario si el jugador quiere que se mueva a la izquierda
	void set_left(bool direction);
	// Le indica a mario si el jugador quiere que se mueva a la derecha
	void set_right(bool direction);
	// Limita el movimiento de mario
	void bound_player(int left_border, int right_border);
	// Si choca contra plataformas lo ponen en su lugar
	void collision(int old_x, int old_y);
	// number
	int get_player_number();
	int get_vidas_restantes();
	int get_puntos();
	bool is_jumping();
	void agregar_puntos(int puntos_a_agregar);

	void disconnect() { connected = false; };
	void reconnect() { connected = true; };
	std::string username;
	bool finished_level = false;
	int puesto_llegada = 0;
};