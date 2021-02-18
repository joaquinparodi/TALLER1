#pragma once

#include <thread>
#include <string>

// Esta classe se usa para tener una abstraccion general para mis threads
// El metodo run tiene que ser overrideado para indicarle a la thread
// que ejecutar
class Thread {
public:
	// Constructores y Destructores
	Thread();
	virtual ~Thread();
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;
	Thread(Thread&& other);
	Thread& operator=(Thread&& other);

	// Metodos de Control de la thread
	// El metodo run virtual contiene el codigo a correr
	void start();
	void join();
	bool joinable();
	virtual void run() = 0;
private:
	std::thread thread;
};
