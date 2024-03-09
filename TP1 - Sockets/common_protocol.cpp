#include "common_protocol.h"

ImpresorPosiciones::ImpresorPosiciones(const uint32_t& posicion_x, const uint32_t& posicion_y):
        posicion_x(posicion_x), posicion_y(posicion_y) {}

void ImpresorPosiciones::imprimir_posiciones() {
    std::cout << posicion_x << " " << posicion_y << std::endl;
}

PosicionGusanoDTO::PosicionGusanoDTO() {}

std::vector<uint8_t> PosicionGusanoDTO::serializar(const uint32_t& posicion_x,
                                                   const uint32_t& posicion_y) {
    std::vector<uint8_t> buffer;

    uint32_t gusano_X = htonl(posicion_x);
    uint32_t gusano_Y = htonl(posicion_y);

    uint8_t* ptr_gusano_x = reinterpret_cast<uint8_t*>(&gusano_X);
    uint8_t* ptr_gusano_y = reinterpret_cast<uint8_t*>(&gusano_Y);

    buffer.insert(buffer.end(), ptr_gusano_x, ptr_gusano_x + sizeof(uint32_t));
    buffer.insert(buffer.end(), ptr_gusano_y, ptr_gusano_y + sizeof(uint32_t));

    return buffer;
}

std::pair<uint32_t, uint32_t> PosicionGusanoDTO::deserializar(const uint8_t* posiciones) {
    uint32_t gusano_X = ntohl(*reinterpret_cast<const uint32_t*>(posiciones));
    posiciones += sizeof(uint32_t);
    uint32_t gusano_Y = ntohl(*reinterpret_cast<const uint32_t*>(posiciones));
    return std::make_pair(gusano_X, gusano_Y);
}

ComandosDTO::ComandosDTO() {}

std::vector<uint8_t> ComandosDTO::serializar(const std::vector<std::string>& comandos,
                                             std::string& comando_actual) {
    std::vector<uint8_t> comandos_codificados;
    uint8_t id_comando;

    if (comandos[0] == "select") {
        id_comando = COMANDO_SELECT;
        comandos_codificados.push_back(id_comando);

        uint16_t largo_nombre = htons(comandos[1].length());

        uint8_t* conversion_largo_nombre = reinterpret_cast<uint8_t*>(&largo_nombre);
        comandos_codificados.insert(comandos_codificados.end(), conversion_largo_nombre,
                                    conversion_largo_nombre + sizeof(uint16_t));

        comandos_codificados.insert(comandos_codificados.end(), comandos[1].begin(),
                                    comandos[1].end());

        comando_actual = "select";

    } else if (comandos[0] == "dir") {
        id_comando = COMANDO_DIR;
        comandos_codificados.push_back(id_comando);
        uint8_t direccion = std::stoi(comandos[1]);
        comandos_codificados.push_back(direccion);

        comando_actual = "dir";
    } else if (comandos[0] == "move") {
        id_comando = COMANDO_MOVE;
        comandos_codificados.push_back(id_comando);

        comando_actual = "move";
    } else if (comandos[0] == "jump") {
        id_comando = COMANDO_JUMP;
        comandos_codificados.push_back(id_comando);
        uint8_t tipo_salto = std::stoi(comandos[1]);
        comandos_codificados.push_back(tipo_salto);

        comando_actual = "jump";
    }
    return comandos_codificados;
}

uint16_t ComandosDTO::conversion_endianess_noths(const uint16_t& bytes) { return ntohs(bytes); }
