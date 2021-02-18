#include "renderer.h"

Renderer::Renderer(Config* config, Logger* logger) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();
	win = SDL_CreateWindow("Taller 1 - Tecnofobicos", 0, 0, config->get_window_width(), config->get_window_height(), 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	map = new MapState(renderer, config, logger);

	SDL_Surface* background = SDL_LoadBMP("assets/images/login_screen.bmp");
	background_texture = SDL_CreateTextureFromSurface(renderer, background);
	SDL_FreeSurface(background);

	background = SDL_LoadBMP("assets/images/wait_screen.bmp");
	wait_texture = SDL_CreateTextureFromSurface(renderer, background);
	SDL_FreeSurface(background);

	camera = map->get_camera();
	font = TTF_OpenFont("assets/fonts/mariof.ttf", 26);
	if (font == NULL) {
		logger->error("Fallo cargar la fuente con el error");
		logger->error(TTF_GetError());
	}
	white = { 255, 255, 255, 255 };
	sound_subsystem = new Sound();
}

Renderer::~Renderer() {
	delete log_in;
	delete sound_subsystem;
	delete map;
	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(wait_texture);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void Renderer::run(Sender* sender, Receiver* receiver, Socket* sock) {
	log_in = new Login(renderer, font, map->render_factor, sock);
	bool quitGame = false;
	SDL_Event main_event;
	SDL_PollEvent(&main_event);
	sen = sender;
	rec = receiver;
	rec->sound_subsystem = sound_subsystem;

	//accepter.start();
	using namespace std::chrono;

	// Esto define el framerate
	using Framerate = duration<steady_clock::rep, std::ratio<1, 60>>;
	auto next = steady_clock::now() + Framerate{ 0 };
	int time_delta = 0;
	int frameTime;

	while (!quitGame && main_event.type != SDL_QUIT) {
		frameTime = SDL_GetTicks();

		if (status == RenderStatus::LOGIN && log_in->logged_in)
			login();

		if (status == RenderStatus::WAITING && rec->state != NULL) {
			status = RenderStatus::PLAYING;
		}

		if (status == RenderStatus::PLAYING && rec->state != NULL && rec->get_game_state() == GameStatus::SCORE) {
			status = RenderStatus::SCORE;
			rec->update_map(map);
			for (auto player : map->get_players()) {
				scoreboard.addScore(player.get_name(), player.get_puntos());
			}
			scoreboard.updateScore();
		}

		if (status == RenderStatus::SCORE && rec->state != NULL && rec->get_game_state() == GameStatus::PLAYING) {
			status = RenderStatus::PLAYING;
		}

		if (status == RenderStatus::PLAYING && rec->state != NULL && rec->get_game_state() == GameStatus::END_GAME_SCORE) {
			status = RenderStatus::ENG_GAME_SCORE;
			rec->update_map(map);
			for (auto player : map->get_players()) {
				scoreboard.addScore(player.get_name(), player.get_puntos());
			}
			scoreboard.updateScore();
		}

		if (status != RenderStatus::ENG_GAME_LOST && rec->state != NULL && rec->get_game_state() == GameStatus::END_GAME_LOSE) {
			status = RenderStatus::ENG_GAME_LOST;
			rec->update_map(map);
			for (auto player : map->get_players()) {
				scoreboard.addScore(player.get_name(), player.get_puntos());
			}
			scoreboard.updateScore();
		}

		if (rec->has_ended()) {
			status = RenderStatus::DISCONNECTED;
		}

		while (SDL_PollEvent(&main_event)) {
			if (main_event.type == SDL_KEYDOWN && main_event.key.keysym.sym == SDLK_F2) {
				sound_subsystem->mute();
			} else {
				switch (status) {
				case RenderStatus::LOGIN:
					quitGame = !log_in->input_login(&main_event);
					break;
				case RenderStatus::WAITING:
					quitGame = !input_wait(&main_event);
					break;
				case RenderStatus::PLAYING:
					quitGame = !input_map(&main_event);
					break;
				case RenderStatus::DISCONNECTED:
					quitGame = !input_disc(&main_event);
					break;
				default:
					break;
				}
				if (quitGame)
					break;
			}
		}
		if (quitGame)
			break;

		switch (status) {
		case RenderStatus::LOGIN:
			render_login();
			break;
		case RenderStatus::WAITING:
			render_wait();
			break;
		case RenderStatus::PLAYING:
			render_map();
			break;
		case RenderStatus::SCORE:
			render_score();
			break;
		case RenderStatus::ENG_GAME_SCORE:
			render_end_game_score();
			break;
		case RenderStatus::DISCONNECTED:
			render_disc();
			break;
		case RenderStatus::ENG_GAME_LOST:
			render_lose();
			break;
		default:
			break;
		}

		// Delay de tiempo
		time_delta = 1;
		next += Framerate{ 1 };
		// Codigo en caso de que se salteen frames
		while (next < steady_clock::now()) {
			time_delta++;
			next += Framerate{ 1 };
		}
		std::this_thread::sleep_until(next);
	}
}

void Renderer::render_login() {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background_texture, NULL, NULL);
	log_in->render();
	SDL_RenderPresent(renderer);
}

void Renderer::login() {
	sen->start();
	rec->start();
	map->user = log_in->input;
	status = RenderStatus::WAITING;
}

bool Renderer::input_wait(SDL_Event* event) {
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
			sen->commands.push(Command::disconnect);
			return false;
		}
	}
	return true;
}

void Renderer::render_wait() {
	SDL_RenderCopy(renderer, wait_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

bool Renderer::input_map(SDL_Event* event) {
	// Este bodoque de texto lee los inputs y se los manda al mario
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_LEFT:
			sen->commands.push(Command::left);
			break;
		case SDLK_RIGHT:
			sen->commands.push(Command::right);
			break;
		case SDLK_UP:
			sen->commands.push(Command::jump);
			break;
		case SDLK_DOWN:
			sen->commands.push(Command::crouch);
			break;
		case SDLK_ESCAPE:
			sen->commands.push(Command::disconnect);
			return false;
			break;
		case SDLK_F1:
			sen->commands.push(Command::test);
			break;
		}
		break;

	case SDL_KEYUP:
		switch (event->key.keysym.sym) {
		case SDLK_LEFT:
			sen->commands.push(Command::no_left);
			break;
		case SDLK_RIGHT:
			sen->commands.push(Command::no_right);
			break;
		case SDLK_DOWN:
			sen->commands.push(Command::no_crouch);
			break;
		}
		break;

	default:
		break;
	}
	return true;
}

void Renderer::render_map() {
	rec->update_map(map);
	for (auto player : map->get_players()) {
		scoreboard.addScore(player.get_name(), player.get_puntos());
	}
	SDL_RenderClear(renderer);
	map->render();
	print_time_remaining(map->time);
	print_level_indicator(map->current_world());
	SDL_RenderPresent(renderer);
}

bool Renderer::input_disc(SDL_Event* event) {
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
			sen->commands.push(Command::disconnect);
			return false;
		}
	}
	return true;
}

void Renderer::render_disc() {
	SDL_RenderClear(renderer);
	if (rec->end_status == EndStatus::GAME_ENDED)
		print_server_status("Game Ended");
	else
		print_server_status("Server Crashed");
	SDL_RenderPresent(renderer);
}

void Renderer::render_score() {

	SDL_RenderClear(renderer);

	print_scoreboard_level();

	SDL_RenderPresent(renderer);
}

void Renderer::render_end_game_score() {
	SDL_RenderClear(renderer);
	print_msg("Felicitaciones!", 200, 0, 180, 60);
	print_msg("Ganador:  " + scoreboard.winner(), 450, 0, 180, 60);
	print_scoreboard_final();
	SDL_RenderPresent(renderer);
}

void Renderer::render_lose() {
	SDL_RenderClear(renderer);
	print_msg("Game Over", 300, 10, 150, 60);
	print_scoreboard_final();
	SDL_RenderPresent(renderer);
}

void Renderer::print_fps(int frame_time) {
	std::string fps_count = std::to_string(1000 / frame_time);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, fps_count.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = 0;	//controls the rect's x coordinate
	text_rect.y = 0;	// controls the rect's y coordinte
	text_rect.w = 100;	// controls the width of the rect
	text_rect.h = 100;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void Renderer::print_time_remaining(int passed_time) {
	std::string time_rem = "time: " + std::to_string(passed_time);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, time_rem.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = 700;	//controls the rect's x coordinate
	text_rect.y = 0;	// controls the rect's y coordinte
	text_rect.w = 100;	// controls the width of the rect
	text_rect.h = 50;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void Renderer::print_level_indicator(int number_of_level) {
	std::string level = "world: " + std::to_string(number_of_level);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, level.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = 550;	//controls the rect's x coordinate
	text_rect.y = 0;	// controls the rect's y coordinte
	text_rect.w = 100;	// controls the width of the rect
	text_rect.h = 50;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void Renderer::print_server_status(const char* status) {
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, status, white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect;						//create a rect
	text_rect.x = 200 * map->render_factor; //controls the rect's x coordinate
	text_rect.y = 350 * map->render_factor; // controls the rect's y coordinte
	text_rect.w = 100 * map->render_factor; // controls the width of the rect
	text_rect.h = 50 * map->render_factor;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void Renderer::print_scoreboard_level() {
	print_msg("LEVEL SCORES", 300, 20, 150, 60);
	int x = 220;
	int y = 150;
	int w = 100;
	int h = 50;
	std::string current_user;
	std::string c_score;
	print_msg("user", x, y, w, h);

	x += 200;
	print_msg("score", x, y, w, h);
	print_msg("_____", 190, 200, 400, 20);
	x = 220;
	y += 80;

	for (auto player_score : scoreboard.board) {
		current_user = player_score->name;
		print_msg(current_user, x, y, w, h);

		x += 200;

		c_score = to_string(player_score->parcialScore.back());

		print_msg(c_score, x, y, w, h);

		x = 220;
		y += 80;
	}
}

void Renderer::print_msg(std::string msg, int x, int y, int w, int h) {

	SDL_Surface* text_surface = TTF_RenderText_Blended(font, msg.c_str(), { 255, 255, 255, 255 });
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect;
	text_rect.x = x;
	text_rect.y = y;
	text_rect.w = w;
	text_rect.h = h;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}


void Renderer::print_scoreboard_final() {
	int x = 120;
	int y = 60;
	int w = 60;
	int h = 20;
	std::string current_user;
	std::string c_score;
	print_msg("user", x, y, w, h);

	x += 150;
	print_msg("level", x, y, w, h);
	x += 150;
	print_msg(" parcial score", x, y, w + 40, h);
	x += 150;
	print_msg("total score", x, y, w + 30, h);
	print_msg("_________________", 100, 80, 600, 20);
	x = 120;

	for (auto player_score : scoreboard.board) {
		y += 45;
		current_user = player_score->name;
		print_msg(current_user, x, y, w, h);

		for (int j = 0; j < player_score->parcialScore.size(); j++) {
			x += 150;
			print_msg(std::to_string(j + 1), x, y, 20, 20);
			x += 150;
			c_score = to_string(player_score->parcialScore[j]);
			print_msg(c_score, x, y, w, h);
			y += 45;
			x = 120;
		}
		if (player_score->parcialScore.size() == 3) {
			y -= 135;
			x += 450;
			c_score = to_string(player_score->totalScore);
			print_msg(c_score, x, y, w, h);
			x = 120;
			y += 135;
			print_msg("_________________", 100, y - 10, 600, 20);
		}

		if (player_score->parcialScore.size() == 2) {
			y -= 90;
			x += 450;
			c_score = to_string(player_score->totalScore);
			print_msg(c_score, x, y, w, h);
			x = 120;
			y += 90;
			print_msg("_________________", 100, y - 10, 600, 20);
		}

		if (player_score->parcialScore.size() == 1) {
			y -= 45;
			x += 450;
			c_score = to_string(player_score->totalScore);
			print_msg(c_score, x, y, w, h);

			x = 120;

			y += 45;
			print_msg("_________________", 100, y - 10, 600, 20);
		}
	}
}