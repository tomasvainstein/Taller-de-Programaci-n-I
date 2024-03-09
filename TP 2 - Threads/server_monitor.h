#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

#include "common_impresor.h"
#include "common_protocol.h"
#include "server_queue.h"

class MonitorJugador {
private:
    std::vector<std::reference_wrapper<Queue<MensajeBroadcast>>> jugadores;
    std::mutex mutex_jugadores;
    uint8_t cantidad_jugadores;

public:
    // Constructor que inicializa la clase MonitorJugador.
    MonitorJugador();

    // Conecta un jugador al monitor, agregando su cola de mensajes.
    void conectar_jugador(Queue<MensajeBroadcast>& cola_jugador);

    // Desconecta a un jugador del monitor, eliminando su cola de mensajes.
    void desconectar_jugador(Queue<MensajeBroadcast>& jugador_cola);

    // Envia un mensaje a todos los jugadores conectados.
    void mandar_mensajes(const MensajeBroadcast& mensaje_broadcast);

    // Obtiene la cantidad de jugadores actualmente conectados.
    void jugadores_conectados(uint8_t& cantidad_jugadores);

    ~MonitorJugador();
};

#endif  // SERVER_MONITOR_H
