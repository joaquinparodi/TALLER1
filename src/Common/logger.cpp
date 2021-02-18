#include "logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <math.h>
using namespace std;

Logger::Logger(int level, const char* filename) {
    log_level = level;
    if (!this->abrirArchivo(filename)){
        fprintf(stderr, "Failed to open log\n");
    }
}

Logger::~Logger() {
    this->cerrarArchivo();
}

void Logger::set_level(int level) {
    log_level = level;
}

void Logger::debug(const char* update) {
    if (log_level > 2) {
        string sUpdate("[DEBUG]: ");
        sUpdate.append(update);
        this->escribir(sUpdate.c_str());
    }
}

void Logger::info(const char* update) {
    if (log_level > 1) {
        string sUpdate("[INFO]: ");
        sUpdate.append(update);
        this->escribir(sUpdate.c_str());
    }
}

void Logger::error(const char* update) {
    string sUpdate("[ERROR]: ");
    sUpdate.append(update);
    this->escribir(sUpdate.c_str());
}

void Logger::escribir(const char* update) {
    char buffer[26];
    int millisec;
    struct tm* tm_info;
    struct timeval tv;

    gettimeofday(&tv, NULL);

    millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
    if (millisec >= 1000) { // Allow for rounding up to nearest second
        millisec -= 1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    strftime(buffer, 26, "%d/%m/%Y %H:%M:%S.", tm_info);
    std::string horaActual(buffer);
    string ms = std::to_string(millisec);

    const char* espacio = " - ";
    const char* new_line = "\n";
    string guion(espacio);
    string sUpdate(update);
    string newLine(new_line);
    string hora = horaActual + ms + guion + sUpdate + newLine;
    archivo_log << hora.c_str();
}
bool Logger::abrirArchivo(const char* filename) {
    archivo_log.open(filename, ios::trunc);
    if (!archivo_log) return false;
    return true;
}

void Logger::cerrarArchivo() {
    archivo_log.close();
}

