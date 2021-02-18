#include "sender.h"

void Sender::run() {
	while (!ended) {
		Command send_command = commands.pop();
		if (send_command == Command::nothing)
			return;
		char buffer[4];
		sock->int_to_big_endian((int) send_command, buffer);
		sock->s_write(buffer, 4);
	}
}

Sender::Sender(Socket* socket) {
	sock = socket;
}

void Sender::end() {
	ended = true;
	commands.push(Command::nothing);
}
