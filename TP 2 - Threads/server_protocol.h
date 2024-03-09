#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <algorithm>
#include <atomic>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "common_protocol.h"
#include "common_socket.h"
#include "server_monitor.h"
#include "server_queue.h"

#define MENSAJE_CANT_JUGADORES 0x06
#define MENSAJE_CHAT 0x09
#define BROADCAST_CHAT 0x05

class MonitorJugador;

class ServerProtocol {
private:
    Socket skt_server;
    std::string comando_actual;
    Queue<MensajeBroadcast>& jugador_cola;
    MonitorJugador& monitor_jugador;
    std::atomic<bool> server_activo;

    void recibir_chat(MensajeBroadcast& mensaje_broadcast);
    void enviar_cantidad_jugadores(const MensajeBroadcast& mensaje_broadcast,
                                   std::vector<char>& buffer);
    void enviar_chat(const MensajeBroadcast& mensaje_broadcast, std::vector<char>& buffer);

public:
    // Constructor que inicializa la clase ServerProtocol con el socket, la cola de mensajes del
    // jugador y el monitor de jugadores.
    ServerProtocol(Socket skt, Queue<MensajeBroadcast>& jugador_cola,
                   MonitorJugador& monitor_jugador);

    // Función para enviar un mensaje al cliente.
    void enviar_mensaje();

    // Función para recibir un mensaje del cliente.
    void recibir_mensaje();

    // Función para notificar el estado de los jugadores conectados (número de jugadores) al
    // cliente.
    void notificar_estado_jugadores();

    // Función para borrar a un jugador desconectado de la lista de jugadores activos.
    void borrar_jugador();

    // Función para verificar el estado activo del servidor.
    bool estado_activo();

    // Función para finalizar el servidor y cerrar la conexión con el cliente.
    void finalizar_servidor();
};

#endif  // SERVER_PROTOCOL_H
