#include "common_impresor.h"

Impresor::Impresor(const MensajeBroadcast& mensaje_broadcast):
        mensaje_broadcast(mensaje_broadcast) {}

void Impresor::imprimir_mensaje() { std::cout << mensaje_broadcast.obtener_mensaje() << std::endl; }

void Impresor::imprimir_cantidad_jugadores() {
    std::string numero_jugadores = std::to_string(mensaje_broadcast.obtener_cantidad_jugadores());
    std::cout << "Jugadores " << numero_jugadores << ", esperando al resto de tus amigos..."
              << std::endl;
}

void Impresor::imprimir_broadcast() {
    if (mensaje_broadcast.obtener_tipo_comando() == 0x09) {
        imprimir_mensaje();
    } else {
        imprimir_cantidad_jugadores();
    }
}
