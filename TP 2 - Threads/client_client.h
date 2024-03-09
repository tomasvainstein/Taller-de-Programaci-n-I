#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "client_protocol.h"
#include "common_impresor.h"
#include "common_protocol.h"

class ClientParser {
private:
    const std::string linea_comando;

public:
    // Constructor que inicializa la clase ClientParser.
    explicit ClientParser(const std::string& linea_comando);

    // Analiza la línea de comando y devuelve un vector de cadenas que representan los comandos
    // identificados.
    void identificar_comandos(std::vector<std::string>& comandos);
};

class Client {
private:
    ClientProtocol client_protocol;

public:
    // Constructor de la clase Client que recibe el nombre del host y el nombre del servicio.
    Client(const std::string& hostname, const std::string& servicename);

    // Procesa un comando en función de la línea de comando proporcionada y el estado del cliente.
    // puede enviar un mensaje, leer mensajes o finalizar el cliente.
    void procesar_comando(const std::string& linea_comando, bool& cliente_activo);
};

#endif  // CLIENT_CLIENT_H
