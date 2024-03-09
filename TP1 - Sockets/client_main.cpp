#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "client_protocol.h"

#define ERROR 1
#define EXITO 0

int main(int argc, char* argv[]) {
    try {
        if (argc == 3) {
            std::string hostname = argv[1];
            std::string servicename = argv[2];
            ClientProtocol client_protocol(hostname, servicename);
            std::string linea_comando = "";

            while (std::getline(std::cin, linea_comando) && !std::cin.eof()) {
                if (linea_comando[0] != '#' && !linea_comando.empty()) {
                    client_protocol.enviar_comando(linea_comando);
                    client_protocol.recibir_salida();
                }
            }
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
