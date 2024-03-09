#include "server_jugador_recibidor.h"

#include "server_protocol.h"

JugadorRecibidor::JugadorRecibidor(ServerProtocol& server_protocol):
        server_protocol(server_protocol), conexion_activa(true) {}

void JugadorRecibidor::run() {
    try {
        while (conexion_activa) {
            server_protocol.recibir_mensaje();
            conexion_activa = server_protocol.estado_activo();
        }
    } catch (const std::exception& err) {
        if (conexion_activa) {
            std::cerr << "Error al recibir" << err.what() << std::endl;
        }
        server_protocol.borrar_jugador();
    }
}

void JugadorRecibidor::stop() { conexion_activa = false; }

bool JugadorRecibidor::is_alive() const { return conexion_activa; }

JugadorRecibidor::~JugadorRecibidor() {}
