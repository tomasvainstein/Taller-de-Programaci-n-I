#include "common_protocol.h"

#include <iostream>

MensajeBroadcast::MensajeBroadcast(uint8_t tipo_comando, const std::string& mensaje,
                                   uint8_t cantidad_jugadores):
        tipo_comando(tipo_comando), mensaje(mensaje), cantidad_jugadores(cantidad_jugadores) {}

MensajeBroadcast::MensajeBroadcast() {}

uint8_t MensajeBroadcast::obtener_tipo_comando() const { return tipo_comando; }

const std::string& MensajeBroadcast::obtener_mensaje() const { return mensaje; }

uint8_t MensajeBroadcast::obtener_cantidad_jugadores() const { return cantidad_jugadores; }

void MensajeBroadcast::set_tipo_comando(uint8_t tipo) { tipo_comando = tipo; }

void MensajeBroadcast::set_mensaje(const std::string& mensaje) { this->mensaje = mensaje; }

void MensajeBroadcast::set_cantidad_jugadores(uint8_t cantidad) { cantidad_jugadores = cantidad; }
