#include "server_jugador.h"

#include "server_jugador_recibidor.h"
#include "server_monitor.h"

Jugador::Jugador(Socket socket, MonitorJugador& monitor):
        hilo_recibidor(server_protocol),
        server_protocol(std::move(socket), jugador_cola, monitor),
        jugador_cola(300),
        conexion_activa(true) {
    monitor.conectar_jugador(jugador_cola);
}

void Jugador::run() {
    server_protocol.notificar_estado_jugadores();
    hilo_recibidor.start();
    try {
        while (conexion_activa) {
            server_protocol.enviar_mensaje();
            conexion_activa = server_protocol.estado_activo();
        }
    } catch (const std::exception& err) {
        if (conexion_activa) {
            std::cerr << "Error al enviar" << err.what() << std::endl;
        }
    }
    hilo_recibidor.join();
}

bool Jugador::is_alive() { return server_protocol.estado_activo(); }

void Jugador::stop() {
    jugador_cola.close();
    conexion_activa = false;
    server_protocol.finalizar_servidor();
}
