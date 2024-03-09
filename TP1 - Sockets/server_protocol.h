#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <fstream>
#include <iostream>
#include <string>

#include "common_socket.h"
#include "server_worms.h"

class ServerProtocol {
private:
    const std::string servicename;
    Socket skt;
    std::string comando_actual;
    Juego juego;

public:
    // Constructor que inicializa la clase ServerProtocol con el nombre de servidor y el nombre de
    // archivo.
    ServerProtocol(const std::string& servicename, const std::string& nombre_archivo);

    // Establece una conexión con un cliente y maneja la interacción con el mismo.
    // Recibe comandos del cliente, envía respuestas y realiza otras operaciones relacionadas con el
    // juego.
    void conectar_con_cliente();

    // Acepta una conexión entrante desde un cliente y devuelve el socket de esa conexión.
    Socket aceptar_conexion();

    // Envía la salida al cliente a través del socket.
    // Prepara y envía datos como la posición del gusano y en algunos casos códigos de respuesta.
    void enviar_salida();

    // Recibe comandos del cliente a través del socket, los procesa y los ejecuta en el juego.
    void recibir_comando(bool* ejecucion_activa);
};

#endif  // SERVER_PROTOCOL_H
