#ifndef SERVER_JUGADOR_RECIBIDOR_H
#define SERVER_JUGADOR_RECIBIDOR_H

#include <atomic>

#include "server_protocol.h"
#include "server_thread.h"

class JugadorRecibidor: public Thread {
private:
    ServerProtocol& server_protocol;
    std::atomic<bool> conexion_activa;

public:
    // Constructor que inicializa la clase JugadorRecibidor con el protocolo del servidor.
    explicit JugadorRecibidor(ServerProtocol& server_protocol);

    // Función principal que se ejecuta en el hilo. Recibe mensajes del cliente.
    void run() override;
    // Detiene la recepción de mensajes y desactiva la conexión.
    void stop() override;

    // Verifica si el hilo del jugador receptor sigue activo
    bool is_alive() const override;

    ~JugadorRecibidor();
};

#endif
