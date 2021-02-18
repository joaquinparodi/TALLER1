#include "client.h"
#include "Client/sender.h"

Client::Client(char* port, char* hostname, Config* config, Logger* main_logger) {
	logger = main_logger;
	socket = new Socket(hostname, port);
	renderer = new Renderer(config, main_logger);
}

Client::~Client() {
	delete socket;
	delete renderer;
}

void Client::run() {
	int result = socket->s_connect();
	if (result != 0){
		logger->error("Connection Failed");
		std::cerr << "The connection to the Server Failed" << "\n";
		return;
	}

	Sender sender(socket);
	Receiver receiver(socket);
	renderer->run(&sender, &receiver, socket);
	sender.end();
	receiver.end();
	socket->s_shutdown();
	if (sender.joinable())
		sender.join();
	if (receiver.joinable())
		receiver.join();
}

// gamestate tiene que recibir de modelo estos datos
/*
Protocolo de estado de juego Todo big endian

	largo del state
	Numero del nivel int32
	Camera
	 - x int32
	 - y int32
	Players
	  - Cantidad
		- X int32
		- Y int32
		- Sprite_index int32
	Enemigos
	  - Cantidad
		- X int32
		- Y int32
		- Type
	Plataformas
	  - Cantidad
		- X int32
		- Y int32
		- Amount int 32
		- Type
	Monedas
	  - Cantidad
		- X int32
		- Y int32

	Ejemplo: https://cdn.discordapp.com/attachments/766067323886764036/782316366790787102/560ebbe7dd0895325c8b458e.png
	[1][20][60][3][2][60][70][2][65][70][2][4]

*/
