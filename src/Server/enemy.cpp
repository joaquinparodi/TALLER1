#include "enemy.h"
#include "map.h"

Enemy::Enemy(ConfigEnemy* c_enemy, int right_wall, Logger* logger) {
	enemy_type = c_enemy->type;
	log = logger;
	// Pone al enemigo en un lugar random
	x = rand() % right_wall;
}

Enemy::~Enemy() {
}

int Enemy::get_x() {
	return x;
}

int Enemy::get_y() {
	return y;
}

int Enemy::width() {
	return w;
}

int Enemy::height() {
	return h;
}

int Enemy::get_type() {
	return enemy_type;
}

EnemyStatus Enemy::get_sprite_index() {
	return sprite_index;
}

void Enemy::move() {
	if (right)
		x += x_speed;
	if (!right)
		x -= x_speed;
	sprite_clock++;
	if (right) {
		sprite_clock++;
		if (sprite_clock > 15) {
			if (sprite_index == EnemyStatus::RUN_RIGHT_2)
				sprite_index = EnemyStatus::RUN_RIGHT_1;
			else
				sprite_index = EnemyStatus::RUN_RIGHT_2;
			sprite_clock = 0;
		}
	} else {
		sprite_clock++;
		if (sprite_clock > 15) {
			if (sprite_index == EnemyStatus::RUN_LEFT_2)
				sprite_index = EnemyStatus::RUN_LEFT_1;
			else
				sprite_index = EnemyStatus::RUN_LEFT_2;
			sprite_clock = 0;
		}
	}
}

void Enemy::bounce_wall(int left_wall, int right_wall) {
	if (x > (right_wall - w))
		right = false;
	if (x < left_wall)
		right = true;
}

bool Enemy::collision_costado(int pos_x, int pos_y, int width, int height) {
	if (pos_x < x + w &&
		pos_x + width > x &&
		pos_y < y + h &&
		pos_y + height > y) {
		return true;
	}
	return false;
}

bool Enemy::collision_arriba(int pos_x, int pos_y, int width, int height, bool is_jumping, int y_speed) {
	if (pos_x < x + w &&
		pos_x + width > x &&
		pos_y < y + h &&
		pos_y + height > y &&
		is_jumping &&
		y_speed > 0) {
		return true;
	}
	return false;
}