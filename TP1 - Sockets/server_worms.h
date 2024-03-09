#ifndef WORMS_H
#define WORMS_H

#include <iostream>
#include <string>
#include <vector>

class Escenario {
private:
    std::string nombre;
    int cantidad_filas;
    int cantidad_columnas;
    int alto_actual;

public:
    int gusano_inicial_x;
    int gusano_inicial_y;

    std::vector<std::vector<char>> escenario;

    // Constructor que inicializa un Escenario con nombre, cantidad de filas y columnas.
    Escenario(std::string& nombre, int& cantidad_filas, int& cantidad_columnas);

    // Constructor por defecto para un Escenario.
    Escenario();

    // Agrega una línea de caracteres al escenario actual, incluyendo cada caracter en una posicion
    // correspondiente del escenario.
    void agregar_linea_escenario(const std::string& linea);

    // getters
    int alto_escenario() const;
    int ancho_escenario() const;
    std::string obtener_nombre_escenario() const;
};

class JuegoParser {
private:
    const std::string nombre_archivo;

public:
    // Constructor que recibe el nombre del archivo.
    explicit JuegoParser(const std::string& nombre_archivo);

    // Lee el archivo de configuración y retorna un vector de Escenarios.
    std::vector<Escenario> iniciar_escenarios();
};

class Juego {
private:
    int gusano_x;
    int gusano_y;
    uint8_t gusano_direccion;
    Escenario escenario_elegido;
    bool escenario_activo;

public:
    // Constructor que inicializa el juego a partir de un archivo.
    explicit Juego(const std::string& nombre_archivo);

    std::vector<Escenario> escenarios;

    // Comprueba si una posición (x, y) en el escenario es válida.
    bool es_posicion_valida(int x, int y);

    // Aplica gravedad al gusano a partir de la posición (x, y).
    void aplicar_gravedad(int x, int y);
    // Cambia la dirección del gusano a partir de la que llega por parámetro.
    void direccionar(const uint8_t& nueva_direccion);
    // Mueve el gusano en la dirección actual.
    void mover();
    // Realiza un salto con las posicones del gusano. Si el salto es para adelante, su sube una
    // posición y se adelante dos. Si el salto es hacia atrás, se suben dos posiciones y después se
    // avanza una.
    void saltar(const uint8_t& comando);

    // Ejecuta un comando en el juego correspondiente al tipo que llega por parámetro (select, move,
    // dir, jump).
    void ejecutar_comando(const uint8_t& id_comando, std::vector<unsigned char>& nombre_escenario,
                          const uint8_t& tipo_comando);

    // getters
    int posicion_gusano_x() const;
    int posicion_gusano_y() const;
    int direccion_gusano_actual() const;
    bool hay_escenario_activo() const;
};

#endif  // WORMS_H
