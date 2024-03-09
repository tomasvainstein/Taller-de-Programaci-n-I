#include "client_client.h"

ClientParser::ClientParser(const std::string& linea_comando): linea_comando(linea_comando) {}

void ClientParser::identificar_comandos(std::vector<std::string>& comandos) {
    std::istringstream linea(linea_comando);
    std::string comando;

    while (linea >> comando) {
        comandos.push_back(comando);
    }
}

Client::Client(const std::string& hostname, const std::string& servicename):
        client_protocol(hostname, servicename) {}

void Client::procesar_comando(const std::string& linea_comando, bool& cliente_activo) {
    ClientParser parser(linea_comando);
    std::vector<std::string> comandos;
    parser.identificar_comandos(comandos);

    if (comandos[0] == "Chat") {
        client_protocol.enviar_mensaje(comandos);
    } else if (comandos[0] == "Read") {
        int cantidad_mensajes = std::stoi(comandos[1]);
        for (int i = 0; i < cantidad_mensajes; i++) {
            MensajeBroadcast mensaje_broadcast;
            client_protocol.recibir_mensajes(mensaje_broadcast);
            Impresor impresor(mensaje_broadcast);
            impresor.imprimir_broadcast();
        }
    } else if (comandos[0] == "Exit") {
        client_protocol.finalizar_cliente();
        cliente_activo = false;
    }
}
