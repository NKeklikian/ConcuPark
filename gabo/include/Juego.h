#ifndef JUEGO_H
#define JUEGO_H

#include "Runnable.h"
#include <string>

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
};

#endif // JUEGO_H
