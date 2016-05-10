#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include "Runnable.h"

class Administrador : public Runnable
{
    public:
        Administrador(int periodo_chequeo);
        virtual ~Administrador();
    protected:
        virtual void _run();
    private:
        void init() {}
        int periodo_chequeo;
};

#endif // ADMINISTRADOR_H
