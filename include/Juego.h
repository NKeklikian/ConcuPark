#ifndef JUEGO_H
#define JUEGO_H

#include <string>
#include "Runnable.h"
#include "Semaforo.h"

class Juego : public Runnable
{
    public:
        Juego(std::string n, int cap, int costo);
        virtual ~Juego();
    protected:
        virtual void _run();
    private:

        std::string nombre;
        int capacidad;
        int costo;

        Semaforo sem_entrada;
        Semaforo sem_entrada_libre;
        Semaforo sem_cobrar;
        Semaforo sem_salida;
        Semaforo sem_salir;

        std::string LOG_JUEGO;
        static int cant;

        void init();
};

#endif // JUEGO_H
