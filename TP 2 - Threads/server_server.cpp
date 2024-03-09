#include "server_server.h"

#include "server_lobby.h"

#define COMANDO_STOP "q"

Server::Server(const std::string& servicename): servicename(servicename) {}

void Server::conectar_con_cliente() {
    MonitorJugador monitor_jugador;
    Lobby lobby(servicename, monitor_jugador);

    std::string linea_comando = "";
    lobby.start();

    while (std::getline(std::cin, linea_comando)) {
        if (linea_comando == COMANDO_STOP) {
            break;
        }
    }

    lobby.stop();
    lobby.join();
}
