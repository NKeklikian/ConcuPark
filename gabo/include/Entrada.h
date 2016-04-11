#ifndef ENTRADA_H
#define ENTRADA_H

#include "Runnable.h"
#include <string>
#include <vector>

class Entrada : public Runnable
{
    public:
        Entrada(std::string nombre, int capacidad);
        virtual ~Entrada();
    protected:

        virtual void _run();

    private:

        std::string nombre;
        int capacidad;
        std::vector<pid_t> personas;
};

#endif // ENTRADA_H
