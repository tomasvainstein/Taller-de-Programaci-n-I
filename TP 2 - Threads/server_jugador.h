#ifndef JUGADOR_H
#define JUGADOR_H

#include <atomic>
#include <string>
#include <utility>

#include "server_jugador_recibidor.h"
#include "server_monitor.h"
#include "server_protocol.h"
#include "server_queue.h"
#include "server_thread.h"

class Jugador: public Thread {
private:
    JugadorRecibidor hilo_recibidor;
    ServerProtocol server_protocol;
    Queue<MensajeBroadcast> jugador_cola;
    std::atomic<bool> conexion_activa;

public:
    // Constructor que inicializa la clase Jugador con el socket y el monitor del jugador.
    Jugador(Socket socket, MonitorJugador& monitor);

    // Función principal que se ejecuta en el hilo. Gestiona la comunicación con el jugador.
    void run() override;
    // Detiene la comunicación con el jugador y finaliza el servidor.
    void stop() override;
    // Verifica si la conexión con el jugador sigue activa.
    bool is_alive();
};

#endif  // JUGADOR_H
