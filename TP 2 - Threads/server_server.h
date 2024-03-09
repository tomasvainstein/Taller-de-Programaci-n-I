#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "server_protocol.h"

class Server {
private:
    const std::string servicename;

public:
    // Constructor que inicializa la clase Server con el nombre de servidor.
    explicit Server(const std::string& servicename);

    // Lanza el hilo Lobby y verifica si se ingresa el comando 'q'
    void conectar_con_cliente();
};

#endif  // SERVER_SERVER_H
