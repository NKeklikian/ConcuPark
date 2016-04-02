#ifndef JUEGO_H
#define JUEGO_H

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
};

#endif // JUEGO_H
