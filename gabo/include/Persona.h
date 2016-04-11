#ifndef PERSONA_H
#define PERSONA_H

#include "Runnable.h"
#include "EventHandler.h"

class Persona : public Runnable, public EventHandler
{
    public:
        Persona(int p);
        virtual ~Persona();
    protected:
        virtual void _run();

        virtual int handleSignal ( int signum );
    private:
        int plata;
};

#endif // PERSONA_H
