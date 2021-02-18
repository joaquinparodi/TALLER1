#pragma once

#include <iostream>
#include <fstream>

class Logger {

private:
    std::ofstream archivo_log;
    int log_level;

public:
    Logger(int level, const char* filename);
    ~Logger();
    void set_level(int level);
    virtual void debug(const char* update);
    virtual void info(const char* update);
    virtual void  error(const char* update);
    void escribir(const char* update);

    bool abrirArchivo(const char* filename);
    void cerrarArchivo();

};
