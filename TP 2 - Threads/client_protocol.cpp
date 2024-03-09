#include "client_protocol.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <netinet/in.h>

#include "common_liberror.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& servicename):
        hostname(hostname),
        servicename(servicename),
        skt_cliente(hostname.c_str(), servicename.c_str()) {}

void ClientProtocol::enviar_mensaje(const std::vector<std::string>& comandos) {
    std::vector<uint8_t> mensaje_codificado;
    uint8_t id_comando = ENVIO_CHAT;
    mensaje_codificado.push_back(id_comando);

    std::string mensaje;
    for (size_t i = 1; i < comandos.size(); i++) {
        mensaje += comandos[i];
        if (i < comandos.size() - 1) {
            mensaje += " ";
        }
    }

    uint16_t largo_mensaje = static_cast<uint16_t>(mensaje.length());
    largo_mensaje = htons(largo_mensaje);

    const uint8_t* conversion_largo_nombre = reinterpret_cast<const uint8_t*>(&largo_mensaje);

    mensaje_codificado.insert(mensaje_codificado.end(), conversion_largo_nombre,
                              conversion_largo_nombre + sizeof(uint16_t));

    std::transform(mensaje.begin(), mensaje.end(), std::back_inserter(mensaje_codificado),
                   [](char c) -> uint8_t { return static_cast<uint8_t>(c); });

    bool was_closed = false;
    skt_cliente.sendall(mensaje_codificado.data(), mensaje_codificado.size(), &was_closed);
    if (was_closed) {
        throw LibError(errno, "El socket se cerró");
    }
}

void ClientProtocol::recibir_chat(MensajeBroadcast& mensaje_broadcast) {
    uint16_t largo_mensaje;
    std::vector<unsigned char> mensaje;
    bool was_closed = false;

    skt_cliente.recvall(&largo_mensaje, sizeof(uint16_t), &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket del cliente se cerró");
    }

    largo_mensaje = ntohs(largo_mensaje);

    mensaje.resize(largo_mensaje);

    skt_cliente.recvall(mensaje.data(), largo_mensaje, &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket del cliente se cerró");
    }

    std::string mensaje_str(mensaje.begin(), mensaje.end());

    mensaje_broadcast.set_mensaje(mensaje_str);
}

void ClientProtocol::recibir_cantidad_jugadores(MensajeBroadcast& mensaje_broadcast) {
    uint8_t cantidad_jugadores;
    bool was_closed = false;

    skt_cliente.recvall(&cantidad_jugadores, sizeof(uint8_t), &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket del cliente se cerró");
    }

    mensaje_broadcast.set_mensaje("");
    mensaje_broadcast.set_cantidad_jugadores(cantidad_jugadores);
}

void ClientProtocol::recibir_mensajes(MensajeBroadcast& mensaje_broadcast) {
    uint8_t tipo_mensaje;
    bool was_closed = false;

    skt_cliente.recvall(&tipo_mensaje, sizeof(uint8_t), &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket del cliente se cerró");
    }

    mensaje_broadcast.set_tipo_comando(tipo_mensaje);

    if (tipo_mensaje == MENSAJE_CHAT) {
        recibir_chat(mensaje_broadcast);

    } else if (tipo_mensaje == MENSAJE_CANT_JUGADORES) {
        recibir_cantidad_jugadores(mensaje_broadcast);

    } else {
        throw std::runtime_error("Mensaje desconocido recibido");
    }
}

void ClientProtocol::finalizar_cliente() {
    skt_cliente.shutdown(2);
    skt_cliente.close();
}
