#pragma once

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

class Socket {
private:
	int soc_fd;
	struct addrinfo* ai = NULL;
	struct addrinfo* ai_result = NULL;

	void init_hints(struct addrinfo* hints);
	explicit Socket(int sock);

public:
	// Este metodo creates a socket TDA. 
	Socket(const char* host, const char* port);
	~Socket();

	Socket(Socket&& other);

	int bind_listen();
	int s_connect();
	Socket s_accept();
	Socket* s_accept_pointer();
	Socket operator()();
	int s_read(char* buffer, int buffer_size);
	int s_write(const char* read_bytes, int n);
	int s_close();
	int s_shutdown();

	static void int_to_big_endian(int num, char* big_endian);
	static unsigned big_endian_to_int(char big_endian[4]);

	void sint_to_big_endian(short int num, char* big_endian);
	short int big_endian_to_sint(char big_endian[2]);
};
