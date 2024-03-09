#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "common_socket.h"

class ClientParser {
private:
    const std::string linea_comando;

public:
    // Constructor que inicializa la clase ClientParser.
    explicit ClientParser(const std::string& linea_comando);

    // Analiza la línea de comando y devuelve un vector de cadenas que representan los comandos
    // identificados.
    std::vector<std::string> identificar_comandos();
};

class ClientProtocol {
private:
    const std::string hostname;
    const std::string servicename;
    Socket skt;
    std::string comando_actual;

public:
    // Constructor que inicializa la clase ClientProtocol con el nombre de host y el nombre de
    // servicio.
    ClientProtocol(const std::string& hostname, const std::string& servicename);

    // Envía un comando al servidor a través del socket utilizando la línea de comando
    // proporcionada.
    void enviar_comando(const std::string& linea_comando);

    // Recibe la salida del servidor a través del socket y la procesa, incluyendo la gestión de
    // errores.
    void recibir_salida();
};

#endif  // CLIENT_PROTOCOL_H
