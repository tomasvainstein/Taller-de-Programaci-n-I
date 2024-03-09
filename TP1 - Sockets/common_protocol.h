#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <netinet/in.h>

#define COMANDO_SELECT 0x01
#define COMANDO_DIR 0x03
#define COMANDO_MOVE 0x04
#define COMANDO_JUMP 0x05
#define ESCENARIO_ERROR 0x01
#define ESCENARIO_EXITO 0x00

class ImpresorPosiciones {
private:
    uint32_t posicion_x;
    uint32_t posicion_y;

public:
    // Constructor de la clase ImpresorPosiciones. La inicializa con la coordenada x y coordenada y
    // de la posición del gusano.
    ImpresorPosiciones(const uint32_t& posicion_x, const uint32_t& posicion_y);

    // Imprime las posiciones del gusano en el formato (x y).
    void imprimir_posiciones();
};

class PosicionGusanoDTO {
public:
    // Constructor de la clase PosicionGusanoDTO.
    PosicionGusanoDTO();

    // Serializa las posiciones del gusano en un vector de bytes.
    std::vector<uint8_t> serializar(const uint32_t& posicion_x, const uint32_t& posicion_y);

    // Deserializa un vector de bytes en las posiciones X e Y del gusano.
    std::pair<uint32_t, uint32_t> deserializar(const uint8_t* posiciones);
};

class ComandosDTO {
public:
    // Constructor de la clase ComandosDTO.
    ComandosDTO();

    // Serializa un vector de comandos y establece el comando actual.
    std::vector<uint8_t> serializar(const std::vector<std::string>& comandos,
                                    std::string& comando_actual);

    // Realiza la conversión de endianess (network to host short).
    uint16_t conversion_endianess_noths(const uint16_t& bytes);
};

#endif  // SERVER_PROTOCOL_H
