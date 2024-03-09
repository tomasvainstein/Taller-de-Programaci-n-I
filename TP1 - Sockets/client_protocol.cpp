#include "client_protocol.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "common_liberror.h"
#include "common_protocol.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& servicename):
        hostname(hostname), servicename(servicename), skt(hostname.c_str(), servicename.c_str()) {}

ClientParser::ClientParser(const std::string& linea_comando): linea_comando(linea_comando) {}

std::vector<std::string> ClientParser::identificar_comandos() {
    std::istringstream linea(linea_comando);
    std::vector<std::string> comandos;
    std::string comando;

    while (linea >> comando) {
        comandos.push_back(comando);
    }

    return comandos;
}

void ClientProtocol::enviar_comando(const std::string& linea_comando) {
    ClientParser parser(linea_comando);
    std::vector<std::string> comandos = parser.identificar_comandos();
    bool was_closed = false;

    ComandosDTO comandos_dto;

    std::vector<uint8_t> comandos_codificados = comandos_dto.serializar(comandos, comando_actual);

    skt.sendall(comandos_codificados.data(), comandos_codificados.size(), &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket se cerró");
    }
}

void ClientProtocol::recibir_salida() {
    bool was_closed = false;

    if (comando_actual == "select") {
        uint8_t codigo_respuesta;
        if (skt.recvall(&codigo_respuesta, sizeof(uint8_t), &was_closed) <= 0 ||
            codigo_respuesta == ESCENARIO_ERROR) {
            throw std::runtime_error("Error en la recepción del comando");
        }
    }

    std::vector<uint8_t> posiciones(sizeof(uint32_t) * 2);
    if (skt.recvall(posiciones.data(), posiciones.size(), &was_closed) <= 0) {
        throw std::runtime_error("Error en la recepción del comando");
    }

    PosicionGusanoDTO posicion_gusano_DTO;

    std::pair<uint32_t, uint32_t> coordenadas = posicion_gusano_DTO.deserializar(posiciones.data());

    ImpresorPosiciones impresor_posiciones(coordenadas.first, coordenadas.second);
    impresor_posiciones.imprimir_posiciones();
}
