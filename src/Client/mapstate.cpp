#include "mapstate.h"

MapState::MapState(SDL_Renderer* main_renderer, Config* config, Logger* main_logger) {
    // Asigna los players y rendere
    renderer = main_renderer;
    logger = main_logger;
    ClientPlayer::create_sprites(main_renderer , main_logger);
    ClientLevel::create_sprites(config, renderer);
    ClientEnemy::create_sprites(renderer);
    ClientPlatform::create_sprites(renderer);
    ClientSurprise::create_sprites(renderer);

    // Esto calcula cuanto mas grande es la pantalla que el mapa
    render_factor = float(config->get_window_height()) / float(LEVEL_HEIGHT);

    font = TTF_OpenFont("assets/fonts/mariof.ttf", 26);
	if (font == NULL) {
		logger->error("Fallo cargar la fuente con el error");
		logger->error(TTF_GetError());
	}
    white = { 255, 255, 255, 255 };
}

MapState::~MapState() {
    ClientPlayer::delete_sprites();
    ClientLevel::delete_sprites();
    ClientEnemy::delete_sprites();
    ClientPlatform::delete_sprites();
    ClientSurprise::delete_sprites();
    TTF_CloseFont(font);
    delete current_level;
}

void MapState::render() {
    if (current_level == NULL) {
        return;
    }
    SDL_RenderCopy(renderer, current_level->get_texture(), &camera, NULL);

    for (ClientEnemy enemy : enemies) {
        SDL_Rect dest_enemy;
        if (true) {
            dest_enemy.x = (enemy.get_x() - camera.x) * render_factor;
            dest_enemy.y = enemy.get_y() * render_factor;
            dest_enemy.w = enemy.width() * render_factor;
            dest_enemy.h = enemy.height() * render_factor;
            SDL_RenderCopy(renderer, enemy.get_texture(), NULL, &dest_enemy);
        }
    }

    for (ClientSurprise surprise : surprises) {
        SDL_Rect dest_surprise;
        if (true) {
            dest_surprise.x = (surprise.get_x() - camera.x) * render_factor;
            dest_surprise.y = surprise.get_y() * render_factor;
            dest_surprise.w = surprise.width() * render_factor;
            dest_surprise.h = surprise.height() * render_factor;
            SDL_RenderCopy(renderer, surprise.get_texture(), NULL, &dest_surprise);
        }
    }

    for (ClientPlatform platform : platforms) {
        for (int i = 0; i < platform.get_amount(); i++) {
            SDL_Rect dest_platform;
            if (true) {
                dest_platform.x = (platform.get_x() + (i * platform.width()) - camera.x) * render_factor;
                dest_platform.y = platform.get_y() * render_factor;
                dest_platform.w = platform.width() * render_factor;
                dest_platform.h = platform.height() * render_factor;
                SDL_RenderCopy(renderer, platform.get_texture(), NULL, &dest_platform);
            }
        }
    }

    //Dibujo a los players
    for (ClientPlayer player : players) {
        if (player.get_name() == user) {
            vidas = player.get_vidas();
            continue;
        }
        SDL_Rect dest_player;
        dest_player.x = (player.get_x() - camera.x) * render_factor;
        dest_player.y = player.get_y() * render_factor;
        dest_player.w = player.width() * render_factor;
        dest_player.h = player.height() * render_factor;
        SDL_RenderCopy(renderer, player.get_texture(), NULL, &dest_player);
        print_player_names(dest_player.x, dest_player.y , player.get_name());
       
	    
    }

    ClientPlayer player = players[current_player];
    SDL_Rect dest_player;
    dest_player.x = (player.get_x() - camera.x) * render_factor;
    dest_player.y = player.get_y() * render_factor;
    dest_player.w = player.width() * render_factor;
    dest_player.h = player.height() * render_factor;
    SDL_RenderCopy(renderer, player.get_texture(), NULL, &dest_player);
    print_player_names(dest_player.x, dest_player.y, player.get_name());
    print_vidas_restantes(player.get_vidas());
    print_puntos(player.get_puntos());
}

void MapState::update(GameState* state) {
    if (current_level)
        delete current_level;
    current_level = new ClientLevel(state->level_number);
    camera.x = state->camera_x;
    camera.y = state->camera_y;
    time = state->time;
    players.clear();
    for (auto player : state->c_players) {
        if (player.get_name() == user) {
            current_player = players.size();
        }
        players.push_back(player);
    }
    enemies.clear();
    for (auto enemy : state->c_enemies) {
        enemies.push_back(enemy);
    }
    platforms.clear();
    for (auto platform : state->c_platforms) {
        platforms.push_back(platform);
    }
    surprises.clear();
    for (auto coin : state->c_surprises) {
        surprises.push_back(coin);
    }

}

int MapState::current_world() {
    return current_level->get_number();
}


SDL_Rect* MapState::get_camera() {
    return &camera;
}
std::vector<ClientPlayer> MapState::get_players(){
    return players;
}

void MapState::print_player_names(int x, int y, std::string name) {
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, name.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = x - 15;	//controls the rect's x coordinate
	text_rect.y = y - 40;	// controls the rect's y coordinte
	text_rect.w = 80;	// controls the width of the rect
	text_rect.h = 30;
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void MapState::print_vidas_restantes(int vidas) {
	std::string level = "vidas: " + std::to_string(vidas);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, level.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = 400;//*render_factor;		//controls the rect's x coordinate
	text_rect.y = 0;//* render_factor;		// controls the rect's y coordinte
	text_rect.w = 100;// * render_factor;	 	// controls the width of the rect
	text_rect.h = 50;// * render_factor;	
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

void MapState::print_puntos(int puntos) {
	std::string level = "puntos: " + std::to_string(puntos);
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, level.c_str(), white);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect text_rect; //create a rect
	text_rect.x = 210; //* render_factor;		//controls the rect's x coordinate
	text_rect.y = 0; //* render_factor;		// controls the rect's y coordinte
	text_rect.w = 150;// * render_factor;	 	// controls the width of the rect
	text_rect.h = 50; //* render_factor;	
	SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}
