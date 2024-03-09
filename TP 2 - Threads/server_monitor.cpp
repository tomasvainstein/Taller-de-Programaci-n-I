#include "server_monitor.h"

#include "server_jugador.h"

MonitorJugador::MonitorJugador(): cantidad_jugadores(0) {}

void MonitorJugador::conectar_jugador(Queue<MensajeBroadcast>& jugador_cola) {
    std::lock_guard<std::mutex> lck(mutex_jugadores);
    jugadores.push_back(jugador_cola);
}

void MonitorJugador::desconectar_jugador(Queue<MensajeBroadcast>& jugador_cola) {
    std::lock_guard<std::mutex> lck(mutex_jugadores);

    auto n = std::find_if(
            jugadores.begin(), jugadores.end(),
            [&jugador_cola](const std::reference_wrapper<Queue<MensajeBroadcast>>& referencia) {
                return &referencia.get() == &jugador_cola;
            });
    if (n != jugadores.end()) {
        jugadores.erase(n);
    }
}

void MonitorJugador::mandar_mensajes(const MensajeBroadcast& mensaje_broadcast) {
    std::lock_guard<std::mutex> lck(mutex_jugadores);

    for (auto cola_actual = jugadores.begin(); cola_actual != jugadores.end();) {
        Queue<MensajeBroadcast>& jugador_cola = cola_actual->get();
        try {
            jugador_cola.push(mensaje_broadcast);
            cola_actual++;
        } catch (const std::exception& err) {
            cola_actual = jugadores.erase(cola_actual);
        }
    }
    Impresor impresor(mensaje_broadcast);
    impresor.imprimir_broadcast();
}

void MonitorJugador::jugadores_conectados(uint8_t& cantidad_jugadores) {
    std::lock_guard<std::mutex> lck(mutex_jugadores);
    cantidad_jugadores = jugadores.size();
}

MonitorJugador::~MonitorJugador() {}
