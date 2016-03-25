#ifndef JUEGO_H
#define JUEGO_H

#include <mutex>
#include "Persona.h"

class Juego
{
    public:
        Juego(int capacidadMaxima, int precio);
        virtual ~Juego();
    protected:
    private:
        int capacidadMaxima;
        int precio;
        std::mutex entrada;
        std::mutex salida;
};

#endif // JUEGO_H
