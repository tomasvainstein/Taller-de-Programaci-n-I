#ifndef SERVER_IMPRESOR_H
#define SERVER_IMPRESOR_H

#include <iostream>
#include <string>

#include "common_protocol.h"

class Impresor {
private:
    const MensajeBroadcast& mensaje_broadcast;

    // Imprime la cantidad de jugadores almacenada en el objeto MensajeBroadcast.
    void imprimir_cantidad_jugadores();

    // Imprime el mensaje almacenado en el objeto MensajeBroadcast.
    void imprimir_mensaje();

public:
    // Constructor que inicializa la clase Impresor con un objeto MensajeBroadcast.
    explicit Impresor(const MensajeBroadcast& mensaje_broadcast);

    // Imprime el contenido del objeto MensajeBroadcast en función de su tipo de comando.
    void imprimir_broadcast();
};

#endif
