#include "server_protocol.h"

#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common_liberror.h"
#include "common_protocol.h"

ServerProtocol::ServerProtocol(const std::string& servicename, const std::string& nombre_archivo):
        servicename(servicename), skt(servicename.c_str()), juego(nombre_archivo) {}

Socket ServerProtocol::aceptar_conexion() { return skt.accept(); }

void ServerProtocol::conectar_con_cliente() {
    Socket socket_cliente = this->aceptar_conexion();

    skt = std::move(socket_cliente);

    bool conexion_activa = true;
    while (conexion_activa) {

        recibir_comando(&conexion_activa);

        if (!conexion_activa) {
            break;
        }
        enviar_salida();

        uint32_t gusano_X = juego.posicion_gusano_x();
        uint32_t gusano_Y = juego.posicion_gusano_y();

        ImpresorPosiciones impresor_posiciones(gusano_X, gusano_Y);

        impresor_posiciones.imprimir_posiciones();
    }
}

void ServerProtocol::enviar_salida() {
    std::vector<uint8_t> buffer;

    bool was_closed = false;

    PosicionGusanoDTO posicion_gusano_DTO;

    buffer = posicion_gusano_DTO.serializar(juego.posicion_gusano_x(), juego.posicion_gusano_y());

    if (comando_actual == "select") {
        uint8_t codigo_respuesta = juego.hay_escenario_activo() ? ESCENARIO_EXITO : ESCENARIO_ERROR;
        buffer.insert(buffer.begin(), codigo_respuesta);
    }

    skt.sendall(buffer.data(), buffer.size(), &was_closed);

    if (was_closed) {
        throw LibError(errno, "El socket se cerró");
    }
    return;
}

void ServerProtocol::recibir_comando(bool* conexion_activa) {
    bool was_closed = false;

    uint8_t id_comando;

    if (skt.recvall(&id_comando, sizeof(uint8_t), &was_closed) <= 0) {
        *conexion_activa = false;
    }

    uint16_t largo_nombre;
    std::vector<unsigned char> nombre_escenario;
    uint8_t tipo_comando = 0;

    if (id_comando == COMANDO_SELECT) {
        if (skt.recvall(&largo_nombre, sizeof(uint16_t), &was_closed) <= 0) {
            *conexion_activa = false;
        }

        ComandosDTO comandos_dto;

        largo_nombre = comandos_dto.conversion_endianess_noths(largo_nombre);

        nombre_escenario.resize(largo_nombre);

        if (skt.recvall(nombre_escenario.data(), largo_nombre, &was_closed) <= 0) {
            *conexion_activa = false;
        }
        comando_actual = "select";

    } else if (id_comando == COMANDO_MOVE) {
        comando_actual = "move";
    } else if (id_comando == COMANDO_DIR) {
        if (skt.recvall(&tipo_comando, sizeof(uint8_t), &was_closed) <= 0) {
            *conexion_activa = false;
        }
        comando_actual = "dir";

    } else if (id_comando == COMANDO_JUMP) {
        if (skt.recvall(&tipo_comando, sizeof(uint8_t), &was_closed) <= 0) {
            *conexion_activa = false;
        }
        comando_actual = "jump";
    }
    juego.ejecutar_comando(id_comando, nombre_escenario, tipo_comando);
    return;
}
