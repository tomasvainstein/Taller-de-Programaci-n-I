#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "common_protocol.h"
#include "common_socket.h"

#define MENSAJE_CHAT 0x09
#define MENSAJE_CANT_JUGADORES 0x06
#define ENVIO_CHAT 0x05

class ClientProtocol {
private:
    const std::string hostname;
    const std::string servicename;
    Socket skt_cliente;
    std::string comando_actual;

public:
    // Constructor que inicializa la clase ClientProtocol con el nombre de host y el nombre de
    // server.
    ClientProtocol(const std::string& hostname, const std::string& servicename);

    // Envía un mensaje al servidor en función de los comandos proporcionados.
    void enviar_mensaje(const std::vector<std::string>& mensaje);

    // Recibe un mensaje de chat a traves del socket y lo almacena en el objeto MensajeBroadcast.
    void recibir_chat(MensajeBroadcast& mensaje_broadcast);

    // Recibe la cantidad de jugadores a través del socket y lo almacena en el objeto
    // MensajeBroadcast referenciado.
    void recibir_cantidad_jugadores(MensajeBroadcast& mensaje_broadcast);

    // Recibe la salida del servidor a través del socket y la procesa, incluyendo la gestión de
    // errors
    void recibir_mensajes(MensajeBroadcast& mensaje_broadcast);

    // Cierra adecuadamente el cliente, apagando y cerrando el socket.
    void finalizar_cliente();
};

#endif  // CLIENT_PROTOCOL_H
