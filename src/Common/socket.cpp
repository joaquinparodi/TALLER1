//#define _POSIX_C_SOURCE 200112L // addrinfo esta aca#include <stdio.h>
//#define _GNU_SOURCE // NI_MAXHOST, NI_MAXSERV
//#define _DEFAULT_SOURCE // NI_MAXHOST, NI_MAXSERV

#include "socket.h"
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include <utility>

// Buffer Size
#define BUFFER_SIZE 512
#define MIN_BUFFER_SIZE 20
#define BACKLOG 10

// Funcion interna que maneja la inizializacion del os hints
void Socket::init_hints(addrinfo* hints) {
    memset(hints, 0, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_flags = AI_PASSIVE;
    hints->ai_protocol = 0;
    hints->ai_canonname = NULL;
    hints->ai_addr = NULL;
    hints->ai_next = NULL;
}

Socket::Socket(int sock) {
    this->soc_fd = sock;
}

void Socket::int_to_big_endian(int num, char* big_endian) {
    big_endian[0] = (num >> 24) & 0xFF;
    big_endian[1] = (num >> 16) & 0xFF;
    big_endian[2] = (num >> 8) & 0xFF;
    big_endian[3] = num & 0xFF;
}

unsigned int Socket::big_endian_to_int(char big_endian[4]) {
    int acumulator = ((unsigned char)big_endian[0] & 0xFF) << 24;
    acumulator += ((unsigned char)big_endian[1] & 0xFF) << 16;
    acumulator += ((unsigned char)big_endian[2] & 0xFF) << 8;
    acumulator += (unsigned char)big_endian[3] & 0xFF;
    return acumulator;
}

void Socket::sint_to_big_endian(short int num, char* big_endian) {
    big_endian[0] = (num >> 8) & 0xFF;
    big_endian[1] = (num) & 0xFF;
}

short int Socket::big_endian_to_sint(char big_endian[2]) {
    short int acumulator = ((unsigned char)big_endian[0] & 0xFF) << 8;
    acumulator += ((unsigned char)big_endian[1] & 0xFF);
    return acumulator;
}

Socket::Socket(const char* host, const char* port) {
    struct addrinfo hints, *results, *ai_current;
    init_hints(&hints);
    int socket_file, status = getaddrinfo(host, port, &hints, &results);
    if (status != 0) throw std::runtime_error("getaddrinfo failed");

    for (ai_current = results; ai_current != NULL; ai_current = ai_current->ai_next) {
        socket_file = socket(ai_current->ai_family, ai_current->ai_socktype, ai_current->ai_protocol);
        if (socket_file != -1) break; // Si se consiguio el socket
    }

    if (ai_current == NULL) { // Termino de iterar sin encontrar un bind
        freeaddrinfo(results);
        throw std::runtime_error("failed to open socket");
    }

    soc_fd = socket_file;
    ai = ai_current;
    ai_result = results;
}

Socket::~Socket() {
    if (ai_result != NULL){
        freeaddrinfo(ai_result);
    }
    if (soc_fd != -1){
        s_shutdown();
    }
}

Socket::Socket(Socket&& other) {
    this->soc_fd = std::move(other.soc_fd);
    other.soc_fd = -1;
    this->ai = other.ai;
    other.ai = nullptr;
    this->ai_result = other.ai_result;
    other.ai_result = nullptr;
}

int Socket::bind_listen() {
    int val = 1;
    setsockopt(soc_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if (bind(this->soc_fd, this->ai->ai_addr, this->ai->ai_addrlen) != 0) {
        return -1;
    }
    return listen(this->soc_fd, BACKLOG);
}

int Socket::s_connect() {
    if (this->soc_fd == -1 || this->ai == NULL || this->ai->ai_addr == NULL) {
        fprintf(stderr, "Fallo s_connect\n");
        return -1;
    }
    return connect(this->soc_fd, this->ai->ai_addr, this->ai->ai_addrlen);
}

Socket Socket::operator()() {
    return Socket(accept(this->soc_fd, NULL, 0));
}

Socket Socket::s_accept() {
    return Socket(accept(this->soc_fd, NULL, 0));
}

Socket* Socket::s_accept_pointer() {
    return new Socket(accept(this->soc_fd, NULL, 0));
}

int Socket::s_read(char* buffer, int buffer_size) {
    return recv(this->soc_fd, buffer, buffer_size, MSG_WAITALL);
}

int Socket::s_write(const char* read_bytes, int n) {
    return send(this->soc_fd, read_bytes, n, MSG_NOSIGNAL);
}

int Socket::s_shutdown() {
    return shutdown(soc_fd, SHUT_RDWR);
}

int Socket::s_close() {
    return close(soc_fd);
}
