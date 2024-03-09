#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "client_client.h"

#define ERROR 1
#define EXITO 0

int main(int argc, char* argv[]) {
    try {
        if (argc == 3) {
            const std::string hostname = argv[1];
            const std::string servicename = argv[2];

            Client client(hostname, servicename);

            std::string linea_comando = "";
            bool cliente_activo = true;

            while (cliente_activo && std::getline(std::cin, linea_comando)) {
                client.procesar_comando(linea_comando, cliente_activo);
            }

            return EXITO;
        } else {
            std::cerr << "Fallo la ejecución"
                      << "\n";
            return ERROR;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "Excepción desconocida." << '\n';
    }
}
