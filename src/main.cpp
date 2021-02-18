//Using SDL and standard IO
#include "Common/logger.h"
#include "server.h"
#include "client.h"
#include <string>
#include <time.h>

int main(int argc, char* args[]) {
	/* initialize random seed: */
	srand(time(NULL));
	std::string config_path = "assets/config.xml";
	Logger logger(3, "output.log");

	// ./exe server (port) *(log_level) *(config)
	// ./exe client (port) (ip) *(log_level) *(config)
	if (argc <= 2) {
		fprintf(stderr, "Faltan parametros\n");
		return 0;
	}

	if (std::string("server").compare(args[1]) == 0) {
		if (argc > 3) {
			logger.set_level(std::stoi(args[3]));
			if (argc > 4) {
				config_path.assign(args[4]);
			}
		}
		Config config(config_path.c_str(), &logger);
		if (argc == 3) {
			logger.set_level(config.get_level());
		}

		Server server(args[2], &config, &logger);
		server.run();
	}

	if (std::string("client").compare(args[1]) == 0) {
		if (argc > 4) {
			logger.set_level(std::stoi(args[3]));
			if (argc > 5) {
				config_path.assign(args[4]);
			}
		}
		Config config(config_path.c_str(), &logger);
		if (argc == 4) {
			logger.set_level(config.get_level());
		}
		
		if (argc <= 3) {
			fprintf(stderr, "Faltan el parametro ip del cliente\n");
			return 0;
		}
		Client client(args[2], args[3], &config, &logger);
		client.run();
	}
	logger.cerrarArchivo();
	return 0;
}