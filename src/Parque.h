#ifndef PARQUE_H_INCLUDED
#define PARQUE_H_INCLUDED
#include "Juego.h"
#include <vector>
#include <mutex>

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
        std::mutex entrada;
        std::mutex salida;
};

#endif // PARQUE_H_INCLUDED
