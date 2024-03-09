#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#include <cstdint>
#include <string>

class MensajeBroadcast {
private:
    uint8_t tipo_comando;
    std::string mensaje;
    uint8_t cantidad_jugadores;

public:
    // Constructor que inicializa la clase MensajeBroadcast con valores proporcionados.
    explicit MensajeBroadcast(uint8_t tipo_comando, const std::string& mensaje = "",
                              uint8_t cantidad_jugadores = 0);

    // Constructor por defecto que inicializa la clase MensajeBroadcast sin valores.
    MensajeBroadcast();

    // Devuelve el tipo de comando del mensaje.
    uint8_t obtener_tipo_comando() const;

    // Devuelve el contenido del mensaje.
    const std::string& obtener_mensaje() const;

    // Devuelve la cantidad de jugadores en el mensaje.
    uint8_t obtener_cantidad_jugadores() const;

    // Establece el tipo de comando del mensaje.
    void set_tipo_comando(uint8_t tipo);

    // Establece el contenido del mensaje.
    void set_mensaje(const std::string& mensaje);

    // Establece la cantidad de jugadores en el mensaje.
    void set_cantidad_jugadores(uint8_t cantidad);
};

#endif
