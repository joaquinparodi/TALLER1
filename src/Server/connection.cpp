#include "connection.h"
#include <string.h>
#include <ostream>

Connection::Connection(Socket* socket) {
	sock = socket;
	start();
}

Connection::~Connection() {
	std::unique_lock<std::mutex> lock(m);
	char end_buf[4];
	sock->int_to_big_endian(-1, end_buf);
	sock->s_write(end_buf, 4);
	delete sock;
	join();
}

void Connection::run() {
	while (!ended){
		if (!validated && !logged_in) {
			//lee usuario y contraseña
			char buffer[4];

			// Largo Usuario
			int result = sock->s_read(buffer, 4);
			if (result != 4) {
				return;
			}
			unsigned int largo = sock->big_endian_to_int(buffer);
			
			// Leer Usuario
			char* buf_user = (char*)malloc(largo+1);
			result = sock->s_read(buf_user, largo);
			if (result != largo) {
				return;
			}
			buf_user[largo] = '\0';
			user.assign(buf_user);
			
			// Largo Password
			result = sock->s_read(buffer, 4);
			if (result != 4) {
				return;
			}
			largo = sock->big_endian_to_int(buffer);
			
			// Leer Constrasña Password
			char* buf_pass = (char*)malloc(largo + 1);
			result = sock->s_read(buf_pass, largo);
			if (result != largo) {
				return;
			}
			buf_pass[largo] = '\0';
			password.assign(buf_pass);

			validated = true;
		}
		if (logged_in) {;
			char buffer_command[4];
			int result = sock->s_read(buffer_command, 4);
			if (result != 4) {
				end();
			}
			Command command_int = (Command)sock->big_endian_to_int(buffer_command);
			commands.push(command_int);
		}
	}
}

void Connection::send_gamestate(GameState* game_state) {
	std::unique_lock<std::mutex> lock(m);
	char* state = game_state->encode();
	if (sock->s_write(state, game_state->lenght) != 1) {
		//end();
	}
	delete state;
}

void Connection::server_filled() {
	char buffer[4];
	sock->int_to_big_endian(2, buffer);
	sock->s_write(buffer, 4);
	validated = false;
}

void Connection::connected() {
	char buffer[4];
	sock->int_to_big_endian(0, buffer);
	sock->s_write(buffer, 4);
	logged_in = true;
}

void Connection::wrong_user_password() {
	char buffer[4];
	sock->int_to_big_endian(1, buffer);
	sock->s_write(buffer, 4);
	validated = false;
}
