#pragma once

#include"../Common/thread.h"
#include"../Common/socket.h"
#include"../Common/command.h"

class Sender : public Thread {
public:
	// El parametro type indica que tipo de productor es
	void virtual run() override;

	Sender(Socket* socket);

	void end();
	CommandQueue commands;
private:
	Socket* sock;
	bool ended = false;
};
