#ifndef SALIDA_H
#define SALIDA_H

#include "Runnable.h"
#include <string>

class Salida : public Runnable
{
    public:
        Salida(std::string nombre);
        virtual ~Salida();
    protected:

        virtual void _run();

    private:

        std::string nombre;
};

#endif // SALIDA_H
