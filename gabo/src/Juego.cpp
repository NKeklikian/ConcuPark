#include "Juego.h"
#include "Cola.h"
#include "Entrada.h"
#include "Salida.h"

Juego::Juego(std::string n, int cap) : nombre(n), capacidad(cap)
{
    //ctor
}

Juego::~Juego()
{
    //dtor
}

void Juego::_run(){
    Cola cola("/tmp/juego");
    cola.run();

    Entrada entrada("/tmp/juego", 5);
    entrada.run();

    Salida salida("/tmp/juego");
    salida.run();
}
