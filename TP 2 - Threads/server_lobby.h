#ifndef SERVER_LOBBY_H_
#define SERVER_LOBBY_H_

#include <algorithm>
#include <atomic>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "common_socket.h"
#include "server_jugador.h"
#include "server_queue.h"
#include "server_thread.h"

class MonitorJugador;

class Lobby: public Thread {
private:
    std::vector<Thread*> jugadores;
    Socket skt_server;
    MonitorJugador& monitor_jugador;
    std::atomic<bool> conexion_activa;

    // Acepta una conexión entrante y devuelve el socket del jugador.
    Socket aceptar_conexion();

public:
    // Constructor que inicializa la clase Lobby con el nombre de servicio y el monitor de
    // jugadores.
    Lobby(const std::string& servicename, MonitorJugador& monitor_jugador);

    // Función principal que ejecuta el lobby para aceptar conexiones de jugadores y gestionarlos.
    void run() override;

    // Detiene el lobby y finaliza la comunicación con los jugadores.
    void stop() override;

    // Verifica si la conexión con el lobby sigue activa.
    bool is_alive() const override;

    // Elimina jugadores que han terminado su comunicación.
    void borrar_jugadores_desconectados();

    // Destruye todos los hilos de jugadores y los elimina del vector.
    void destruir_clientes();
};

#endif
