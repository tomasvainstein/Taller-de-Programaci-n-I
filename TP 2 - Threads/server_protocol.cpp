#include "server_protocol.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <netinet/in.h>

#include "common_liberror.h"

ServerProtocol::ServerProtocol(Socket skt_server, Queue<MensajeBroadcast>& jugador_cola,
                               MonitorJugador& monitor_jugador):
        skt_server(std::move(skt_server)),
        jugador_cola(jugador_cola),
        monitor_jugador(monitor_jugador),
        server_activo(true) {}

void ServerProtocol::enviar_cantidad_jugadores(const MensajeBroadcast& mensaje_broadcast,
                                               std::vector<char>& buffer) {
    buffer.push_back(mensaje_broadcast.obtener_cantidad_jugadores());
}

void ServerProtocol::enviar_chat(const MensajeBroadcast& mensaje_broadcast,
                                 std::vector<char>& buffer) {
    std::string mensaje = mensaje_broadcast.obtener_mensaje();

    uint16_t largo_mensaje = static_cast<uint16_t>(mensaje_broadcast.obtener_mensaje().length());
    largo_mensaje = htons(largo_mensaje);

    const uint8_t* conversion_largo_nombre = reinterpret_cast<const uint8_t*>(&largo_mensaje);

    buffer.insert(buffer.end(), conversion_largo_nombre,
                  conversion_largo_nombre + sizeof(uint16_t));

    std::transform(mensaje.begin(), mensaje.end(), std::back_inserter(buffer),
                   [](char c) -> uint8_t { return static_cast<uint8_t>(c); });
}

void ServerProtocol::enviar_mensaje() {
    MensajeBroadcast mensaje_broadcast = jugador_cola.pop();
    uint8_t tipo_mensaje = mensaje_broadcast.obtener_tipo_comando();
    std::vector<char> buffer;
    bool was_closed = false;

    buffer.push_back(tipo_mensaje);

    if (tipo_mensaje == MENSAJE_CANT_JUGADORES) {
        enviar_cantidad_jugadores(mensaje_broadcast, buffer);
    } else if (tipo_mensaje == MENSAJE_CHAT) {
        enviar_chat(mensaje_broadcast, buffer);
    }

    if (server_activo) {
        skt_server.sendall(buffer.data(), buffer.size(), &was_closed);
    }

    if (was_closed) {
        server_activo = false;
        throw std::runtime_error("Error al enviar");
    }
}

void ServerProtocol::recibir_chat(MensajeBroadcast& mensaje_broadcast) {
    uint16_t largo_mensaje;
    std::vector<unsigned char> mensaje;
    bool was_closed = false;

    skt_server.recvall(&largo_mensaje, sizeof(uint16_t), &was_closed);

    if (was_closed) {
        server_activo = false;
        throw std::runtime_error("Error al recibir");
    }

    largo_mensaje = ntohs(largo_mensaje);
    mensaje.resize(largo_mensaje);

    skt_server.recvall(mensaje.data(), largo_mensaje, &was_closed);

    if (was_closed) {
        server_activo = false;
        throw std::runtime_error("Error al recibir");
    }

    std::string mensaje_str(mensaje.begin(), mensaje.end());

    mensaje_broadcast.set_tipo_comando(MENSAJE_CHAT);
    mensaje_broadcast.set_mensaje(mensaje_str);
    monitor_jugador.mandar_mensajes(mensaje_broadcast);
}

void ServerProtocol::recibir_mensaje() {
    uint8_t tipo_mensaje;
    bool was_closed = false;
    MensajeBroadcast mensaje_broadcast;

    skt_server.recvall(&tipo_mensaje, sizeof(uint8_t), &was_closed);

    if (was_closed) {
        server_activo = false;
        throw std::runtime_error("Error al recibir");
    }

    if (tipo_mensaje == BROADCAST_CHAT) {
        recibir_chat(mensaje_broadcast);
    }
}

void ServerProtocol::borrar_jugador() {
    monitor_jugador.desconectar_jugador(jugador_cola);
    notificar_estado_jugadores();
}

void ServerProtocol::notificar_estado_jugadores() {
    MensajeBroadcast mensaje_broadcast;
    mensaje_broadcast.set_tipo_comando(MENSAJE_CANT_JUGADORES);

    uint8_t cantidad_jugadores;
    monitor_jugador.jugadores_conectados(cantidad_jugadores);

    mensaje_broadcast.set_cantidad_jugadores(cantidad_jugadores);
    monitor_jugador.mandar_mensajes(mensaje_broadcast);
}

bool ServerProtocol::estado_activo() { return server_activo; }

void ServerProtocol::finalizar_servidor() {
    server_activo = false;
    skt_server.shutdown(2);
    skt_server.close();
}
