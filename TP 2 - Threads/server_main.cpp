#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "server_server.h"

#define ERROR 1
#define EXITO 0

int main(int argc, char* argv[]) {
    try {
        if (argc == 2) {
            std::string servicename = argv[1];
            Server server(servicename);

            server.conectar_con_cliente();

            return EXITO;
        } else {
            std::cerr << "Fallo la ejecución"
                      << "\n";
            return ERROR;
        }
    } catch (const std::exception& err) {
        std::cerr << "Fallo" << err.what() << "\n";
        return ERROR;
    } catch (...) {
        std::cerr << "Excepción desconocida."
                  << "\n";
        return ERROR;
    }
}
