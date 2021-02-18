#pragma once

#include <list>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include "clientplayer.h"
#include "clientlevel.h"
#include "../Common/logger.h"
#include "../Common/config.h"
#include "../Server/map.h"

// El mapa es el responsable de organizor todos los actores en el mapas
// Y de renderizarlo
class MapState {
private:
    //std::list<Enemy> enemies;
    //std::list<Platform> platforms;
    SDL_Renderer* renderer;
    std::vector<ClientPlayer> players;
    std::vector<ClientSurprise> surprises;
    std::vector<ClientPlatform> platforms;
    std::vector<ClientEnemy> enemies;
    Logger* logger;
    TTF_Font* font;
    SDL_Color white;

    // Administracion de niveles
    int level_index = 0;
    ClientLevel* current_level = NULL;
    int vidas = 3;
    int puntos = 0;
    int current_player = 0;

    // Piso del nivel
    int y_floor = 0;

public:
    MapState(SDL_Renderer* main_renderer, Config* config, Logger* main_logger);
    ~MapState();

    // Deberia ser ejecutado una vez por frame
    // Este metodo pone al fondo enemigos y ladrillos en pantalla
    void render();
    void update(GameState* state);
    std::string user;

    // Guarda la posicion de la camara, y lo utiliza para poner el fondo
    SDL_Rect camera = { 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT };
    float render_factor = 1;

    int time = 0;
    // Devuelve el numero del nivel
    int current_world();
    //Devuelve la cantidad de vida restante
    int current_life();
    //devuelve la cantidad de puntos acumulados
    int level_points();
    SDL_Rect* get_camera();
    void print_player_names(int x, int y, std::string name);
    void print_vidas_restantes(int vidas);
    void print_puntos(int puntos);
    std::vector<ClientPlayer> get_players();

};