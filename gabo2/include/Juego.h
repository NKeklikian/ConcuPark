#ifndef JUEGO_H
#define JUEGO_H

#include <string>
#include "Runnable.h"
#include "Semaforo.h"

class Juego : public Runnable
{
    public:
        Juego(std::string n, int cap);
        virtual ~Juego();
    protected:
        virtual void _run();
    private:

        std::string nombre;
        int capacidad;

        Semaforo sem_entrada;
        Semaforo sem_cobrar;
        Semaforo sem_salida;
        Semaforo sem_salir;

        void init();
};

#endif // JUEGO_H
