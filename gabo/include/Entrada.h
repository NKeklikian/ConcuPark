#ifndef ENTRADA_H
#define ENTRADA_H

#include "Runnable.h"
#include <unistd.h>
#include <string>
#include <vector>

class Entrada : public Runnable
{
    public:
        Entrada(std::string nombre, int capacidad, pid_t cola);
        virtual ~Entrada();
    protected:

        virtual void _run();

    private:

        std::string nombre;
        int capacidad;
        std::vector<pid_t> personas;
        pid_t cola_pid;
};

#endif // ENTRADA_H
