#pragma once
#include <list>
#include <vector>
#include <set>
#include "player.h"
#include "enemy.h"
#include "level.h"
#include "../Common/config.h"
#include "../Common/gamestate.h"

const int CAMERA_WIDTH = 640;
const int CAMERA_HEIGHT = 480;
const int LEVEL_HEIGHT = 480;

class Camera {
public:
    int x = 0;
    int y = 0;
    int width = CAMERA_WIDTH;
    int height = CAMERA_HEIGHT;
};

// El mapa es el responsable de organizor todos los actores en el mapas
// Y de renderizarlo
class Map {
private:
    Logger* logger;
    int y_floor = 200;

    // Administracion de niveles
    std::vector<Level*> levels;
    Level* current_level;
    int level_amount = 0;
    int level_index = 0;
    int clock = 1001;
    int contador_llegada = 0;

    // Guarda la posicion de la camara, y lo utiliza para poner el fondo
    Camera camera;
public:
    Map(Config* config, Logger* main_logger);
    ~Map();

    std::set<Player*> players;
    // Cambia el nivel al siguiente
    // Si el nivel es ultimo devuelve false
    // En otro caso devuelve true
    bool next_level();
    void update();
    bool test_mode = false;

    // Showing Score
    GameStatus status = GameStatus::PLAYING;
    int time_remaining = 0;

    // Devuelve el numero del nivel
    Level* get_current_level();
    Camera* get_camera();

    // Sonido
    std::vector<SoundType> sounds;
};