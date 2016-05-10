#ifndef CAJA_H
#define CAJA_H

#include "Semaforo.h"
#include "MemoriaCompartida.h"

class Caja
{
    public:
        static Caja* getInstance();
        virtual ~Caja();

        int crear();
        void eliminar();

        int getPlata();
        void addPlata(int cant);

    protected:
    private:
        Caja();
        Caja* instancia;

        Semaforo sem_caja;
        MemoriaCompartida<int> plata;
};

#endif // CAJA_H
