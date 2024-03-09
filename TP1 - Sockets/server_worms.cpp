#include "server_worms.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "common_liberror.h"

#define SALTO_ADELANTE 0
#define SALTO_ATRAS 1
#define DIRECCION_ADELANTE 1
#define DIRECCION_ATRAS 0
#define COMANDO_SELECT 0x01
#define COMANDO_DIR 0x03
#define COMANDO_MOVE 0x04
#define COMANDO_JUMP 0x05

Escenario::Escenario(std::string& nombre, int& cantidad_filas, int& cantidad_columnas):
        nombre(nombre),
        cantidad_filas(cantidad_filas),
        cantidad_columnas(cantidad_columnas),
        alto_actual(0),
        gusano_inicial_x(0),
        gusano_inicial_y(0) {
    escenario.resize(cantidad_filas, std::vector<char>(cantidad_columnas, ' '));
}

JuegoParser::JuegoParser(const std::string& nombre_archivo): nombre_archivo(nombre_archivo) {}

std::vector<Escenario> JuegoParser::iniciar_escenarios() {
    std::vector<Escenario> escenarios;

    std::ifstream archivo(nombre_archivo);
    if (!archivo) {
        throw std::runtime_error("Error al iniciar escenarios");
    }
    std::string linea_archivo;
    std::string nombre;
    int cantidad_filas = 0;
    int cantidad_columnas = 0;

    while (std::getline(archivo, linea_archivo)) {
        if (linea_archivo[0] != '#' && !linea_archivo.empty()) {
            if (std::isdigit(linea_archivo[0])) {
                std::istringstream iss(linea_archivo);
                if (!(iss >> cantidad_columnas >> cantidad_filas >> nombre)) {
                    throw std::runtime_error("Error al leer la línea del escenario");
                }
                Escenario nuevo_escenario(nombre, cantidad_filas, cantidad_columnas);
                escenarios.push_back(nuevo_escenario);
                nombre.clear();
                cantidad_filas = 0;
                cantidad_columnas = 0;

            } else {
                escenarios.back().agregar_linea_escenario(linea_archivo);
            }
        }
    }
    return escenarios;
}

Juego::Juego(const std::string& nombre_archivo):
        gusano_x(0), gusano_y(0), gusano_direccion(DIRECCION_ATRAS), escenario_activo(false) {
    JuegoParser parser(nombre_archivo);

    escenarios = parser.iniciar_escenarios();
}

Escenario::Escenario():
        cantidad_filas(0),
        cantidad_columnas(0),
        alto_actual(0),
        gusano_inicial_x(0),
        gusano_inicial_y(0) {}


int Escenario::alto_escenario() const { return cantidad_filas; }

int Escenario::ancho_escenario() const { return cantidad_columnas; }

std::string Escenario::obtener_nombre_escenario() const { return nombre; }

void Escenario::agregar_linea_escenario(const std::string& linea) {
    if (linea.size() != (size_t)cantidad_columnas) {
        throw std::runtime_error("Tamaño de línea incorrecto para el escenario");
    }
    if (alto_actual < cantidad_filas) {
        if (linea.size() != static_cast<size_t>(cantidad_columnas)) {
            throw std::runtime_error("Tamaño de línea incorrecto para el escenario");
        }
        for (int col = 0; col < cantidad_columnas; ++col) {

            if (linea[col] == 'G') {
                gusano_inicial_x = col;
                gusano_inicial_y = alto_actual;
            } else {
                escenario[alto_actual][col] = linea[col];
            }
        }
        alto_actual++;
    } else {
        throw std::runtime_error("El escenario ya está completo");
    }
}

void Juego::direccionar(const uint8_t& nueva_direccion) { gusano_direccion = nueva_direccion; }

bool Juego::es_posicion_valida(int x, int y) {
    int ancho = escenario_elegido.ancho_escenario();
    int alto = escenario_elegido.alto_escenario();

    if (x >= 0 && x < ancho && y >= 0 && y < alto) {
        return escenario_elegido.escenario[y][x] != 'X';
    }

    return false;
}

void Juego::aplicar_gravedad(int x, int y) {
    while (y + 1 < escenario_elegido.alto_escenario() && es_posicion_valida(x, y + 1)) {
        y++;
    }
    gusano_y = y;
}


void Juego::mover() {
    if (gusano_direccion == DIRECCION_ADELANTE && es_posicion_valida(gusano_x + 1, gusano_y)) {
        gusano_x++;
    } else if (gusano_direccion == DIRECCION_ATRAS && es_posicion_valida(gusano_x - 1, gusano_y)) {
        gusano_x--;
    }
    aplicar_gravedad(gusano_x, gusano_y);
}

void Juego::saltar(const uint8_t& comando) {
    int salto_x;
    int salto_y = -1;
    int nueva_x;
    int nueva_y;

    if (comando == SALTO_ADELANTE) {
        salto_x = (gusano_direccion == DIRECCION_ADELANTE) ? 1 : -1;
        nueva_y = gusano_y + salto_y;
        nueva_x = gusano_x;
        if (es_posicion_valida(nueva_x, nueva_y)) {
            gusano_y = nueva_y;
            nueva_x = gusano_x + salto_x;
            if (es_posicion_valida(nueva_x, nueva_y)) {
                gusano_x = nueva_x;
                nueva_x = gusano_x + salto_x;
                if (es_posicion_valida(nueva_x, nueva_y)) {
                    gusano_x = nueva_x;
                }
            }
        }
    } else if (comando == SALTO_ATRAS) {
        salto_x = (gusano_direccion == DIRECCION_ADELANTE) ? -1 : 1;
        nueva_y = gusano_y + salto_y;
        nueva_x = gusano_x;
        if (es_posicion_valida(nueva_x, nueva_y)) {
            gusano_y = nueva_y;
            nueva_y = gusano_y + salto_y;
            if (es_posicion_valida(nueva_x, nueva_y)) {
                gusano_y = nueva_y;
                nueva_x = gusano_x + salto_x;
                if (es_posicion_valida(nueva_x, nueva_y)) {
                    gusano_x = nueva_x;
                }
            }
        }
    }
    aplicar_gravedad(gusano_x, gusano_y);
}

int Juego::posicion_gusano_x() const { return gusano_x; }

int Juego::posicion_gusano_y() const { return gusano_y; }

int Juego::direccion_gusano_actual() const { return gusano_direccion; }

bool Juego::hay_escenario_activo() const { return escenario_activo; }

void Juego::ejecutar_comando(const uint8_t& id_comando,
                             std::vector<unsigned char>& nombre_escenario,
                             const uint8_t& tipo_comando) {
    if (id_comando == COMANDO_SELECT) {
        std::string nombre_str(nombre_escenario.begin(), nombre_escenario.end());

        auto iteracion = std::find_if(escenarios.begin(), escenarios.end(),
                                      [&nombre_str](const Escenario& escenario) {
                                          return escenario.obtener_nombre_escenario() == nombre_str;
                                      });

        if (iteracion != escenarios.end()) {
            escenario_elegido = *iteracion;
            escenario_activo = true;
        }
        gusano_x = escenario_elegido.gusano_inicial_x;
        gusano_y = escenario_elegido.gusano_inicial_y;

    } else if (id_comando == COMANDO_MOVE) {
        mover();
    } else if (id_comando == COMANDO_DIR) {
        direccionar(tipo_comando);
    } else if (id_comando == COMANDO_JUMP) {
        saltar(tipo_comando);
    }
}
