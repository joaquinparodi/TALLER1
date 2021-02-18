#include "player.h"
#include "map.h"
#include <unistd.h>

Player::Player(Logger* logger, std::string user, int number_of_player) {
	player_number = number_of_player;
	username.assign(user);
	modo = PlayerMode::SMALL;
	puntos = 0;
	cantidad_vidas = 3;
	x = 20;
	y = 350;
	w = 32;
	h = 64;
	h_short = 32;
	y_speed = 0;
	x_speed = 0;
	y_accel = 1;
}

Player::~Player() {
}

void Player::respawn() {
	x = 0;
	x_speed = 0;
	last_x = 0;
	last_y = 0;
	right = false;
}

void Player::move() {
	if (!alive)
		return;

	x += x_speed;
	y += y_speed;
	y_speed += y_accel;

	if ((right || left) && sprite_index != PlayerStatus::JUMP) {
		sprite_clock++;
		if (sprite_clock > 3) {
			sprite_index = (PlayerStatus)((int)sprite_index + 1);
			sprite_counter++;
			if (sprite_counter > sprite_limit) {
				if (right)
					sprite_index = PlayerStatus::RUN_RIGHT_1;
				if (left)
					sprite_index = PlayerStatus::RUN_LEFT_1;
				sprite_counter = 0;
			}
		}
	} 
}

int Player::get_vidas_restantes() {
	return cantidad_vidas;
}

int Player::get_puntos() {
	return puntos;
}

int Player::get_x() {
	return x;
}

bool Player::is_jumping() {
	return jumping;
}

int Player::get_y() {
	return y;
}

int Player::width() {
	return w;
}

int Player::get_y_speed() {
	return y_speed;
}

int Player::height() {
	if (is_bending && modo == PlayerMode::BIG)
		return h_short;
	if (is_bending)
		return h_short / 2;
	if (modo == PlayerMode::BIG)
		return h;
	return h / 2;
}

void Player::restar_vida() {
	cantidad_vidas--;
	if (cantidad_vidas == 0) {
		alive = false;
		y = LEVEL_HEIGHT + 64;
	}
}

void Player::agregar_puntos(int puntos_a_agregar) {
	puntos += puntos_a_agregar;
}

void Player::subir_modo() {
	if(modo == PlayerMode::SMALL){
		modo = PlayerMode::BIG;
		y -= height()+10;
	}
}

bool Player::is_connected(){
	return connected;
}

bool Player::is_alive() {
	return alive;
}

void Player::restar_modo() {
	if(modo == PlayerMode::SMALL) {
		restar_vida();
		return;
	}
	modo = PlayerMode::SMALL;
}

void Player::jump() {
	if (!jumping) {
		y_speed = -20;
		jumping = true;
	}
	sprite_index = PlayerStatus::JUMP;
}

void Player::hit_floor(int y_floor) {
	y = y_floor;
	y_speed = 0;
	if (jumping)
		sprite_index = PlayerStatus::STOP;
	jumping = false;
	last_x = x;
	last_y = y;
}

void Player::fall_pit(bool test_mode) {
	if(test_mode == false)
		restar_modo();
	x = last_x;
	y = last_y;
	y_speed = 0;
	x_speed = 0;
}

// Si queres ir a la izquierdo le tenes que restar a la posicion x
// Cuando se termina de mover le sacamos el componente de velocidad
// Verificamos que no se mando varias veces el metodo guardando el estado en una variable booleana left
// ver metodo move()
void Player::set_left(bool direction) {
	if (direction && !left) {
		x_speed -= PLAYER_VEL;
		sprite_index = PlayerStatus::RUN_LEFT_1;
	} else if (!direction && left) {
		x_speed += PLAYER_VEL;
		sprite_index = PlayerStatus::STOP;
	}
	left = direction;
}

// Si queres ir a la derecha le tenes que sumar a la posicion x
// Cuando se termina de mover le sacamos el comoponente de velocidad
// Verificamos que no se mando varias veces el metodo guardando el estado en una variable booleana right
// ver metodo move()
void Player::set_right(bool direction) {
	if (direction && !right) {
		x_speed += PLAYER_VEL;
		sprite_index = PlayerStatus::RUN_RIGHT_1;
	} else if (!direction && right) {
		x_speed -= PLAYER_VEL;
		sprite_index = PlayerStatus::STOP;
	}
	right = direction;
}

void Player::bend(bool bending) {
	if (bending && !is_bending) {
		h = h/2;
		sprite_index = PlayerStatus::BEND;
		y += h/2;
	} else if (!bending && is_bending) {
		h = h*2;
		y -= h/2;
		sprite_index = PlayerStatus::STOP;
	}
	is_bending = bending;
}

void Player::bound_player(int left_border, int right_border) {
	if (x < left_border)
		x = left_border;
	if (x > (right_border - this->width()))
		x = right_border - this->width();
}

void Player::collision(int old_x, int old_y) {
	if (x < old_x)
		x += 1;
	else if (x > old_x)
		x -= 1;
	if (y < old_y)
		y += 1;
	else if (y > old_y) {
		y -= 1;
		hit_floor(old_y);
	}
}

int Player::get_sprite_index() {
	return (int)sprite_index;
}

int Player::get_player_number(){
	if (connected)
		return player_number;
	else
		return 0;
}