#include <iostream>
#include <utility>
#include "accepter.h"

Accepter::Accepter(char* port, Validator* main_validator)
	: sock("0", port){
	sock.bind_listen();
	validator = main_validator;
	start();
}

Accepter::~Accepter() {
}

void Accepter::run() {
	while (!ended) {
		Socket* new_sock = sock.s_accept_pointer();
		Connection* new_connection = new Connection(new_sock);
		validator->add_connection(new_connection);
	}
}

void Accepter::end() {
	ended = true;
	sock.s_shutdown();
}
