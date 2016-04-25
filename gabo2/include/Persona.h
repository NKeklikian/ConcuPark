#ifndef PERSONA_H
#define PERSONA_H

#include "Runnable.h"
#include "EventHandler.h"
#include <vector>
#include <string>

class Persona : public Runnable
{
    public:
        Persona(int p);
        virtual ~Persona();
        void loadInfoParque(std::vector<std::string> &nom_juegos, std::vector<int> &costo_juegos);
    protected:
        virtual void _run();

    private:
        int plata;
        std::vector<std::string> nom_juegos;
        std::vector<int> costo_juegos;

        void init();
};

#endif // PERSONA_H
