#pragma once

//#include"server_number_queue.h"
#include"../Common/thread.h"
#include"connection.h"
#include"../Common/socket.h"
#include<string>
#include<vector>
#include<tuple>
#include "validator.h"

// El accepter es una clase que tiene la responsabilidad de abrir
// y cerrar las conexiones de red cuando termina el programa
// Las que cortan prematuramente hay que terminarlas

class Accepter : public Thread {
public:
	// El parametro type indica que tipo de productor es
	Accepter(char* port, Validator* validator);
	~Accepter();

	virtual void run() override;

	void end();
private:

	Socket sock;
	Validator* validator;
	bool ended = false;
};
