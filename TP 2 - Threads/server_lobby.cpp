#include "server_lobby.h"

Lobby::Lobby(const std::string& servicename, MonitorJugador& monitor_jugador):
        skt_server(servicename.c_str()), monitor_jugador(monitor_jugador), conexion_activa(true) {}

Socket Lobby::aceptar_conexion() { return skt_server.accept(); }

void Lobby::run() {
    try {
        while (conexion_activa) {
            Socket skt_jugador = this->aceptar_conexion();
            Thread* jugador = new Jugador(std::move(skt_jugador), monitor_jugador);
            jugadores.push_back(jugador);

            jugador->start();
            borrar_jugadores_desconectados();
        }
    } catch (const std::exception& err) {
        if (conexion_activa) {
            std::cerr << "Error al recibir" << err.what() << std::endl;
        }
        destruir_clientes();
        conexion_activa = false;
    }
}

void Lobby::borrar_jugadores_desconectados() {
    auto terminado = [](Thread* jugador) {
        if (!jugador->is_alive()) {
            jugador->stop();
            jugador->join();
            delete jugador;
            return true;
        }
        return false;
    };
    jugadores.erase(std::remove_if(jugadores.begin(), jugadores.end(), terminado), jugadores.end());
}

void Lobby::destruir_clientes() {
    for (Thread* jugador: jugadores) {
        jugador->stop();
        jugador->join();
        delete jugador;
    }
    jugadores.clear();
}

void Lobby::stop() {
    conexion_activa = false;
    skt_server.shutdown(2);
    skt_server.close();
}

bool Lobby::is_alive() const { return conexion_activa; }
