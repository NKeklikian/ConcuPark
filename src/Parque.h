#ifndef PARQUE_H_INCLUDED
#define PARQUE_H_INCLUDED
#include "Juego.h"
#include <vector>

class Parque
{
    public :
        Parque();
        virtual ~Parque();
    protected:
    private:
        std::vector<Juego*> juegos;
        int cantidadDePersonas;
        int recaudacionEnCaja;
};

#endif // PARQUE_H_INCLUDED
