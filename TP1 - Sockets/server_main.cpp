#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "server_protocol.h"

#define ERROR 1
#define EXITO 0

int main(int argc, char* argv[]) {
    try {
        if (argc == 3) {
            std::string servicename = argv[1];
            ServerProtocol server_protocol(servicename, argv[2]);
            server_protocol.conectar_con_cliente();

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
