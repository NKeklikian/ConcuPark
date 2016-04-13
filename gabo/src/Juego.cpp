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
    pid_t cola_pid = cola.run();

    Entrada entrada("/tmp/juego", capacidad, cola_pid);
    pid_t entrada_pid = entrada.run();

    Salida salida("/tmp/juego");
    pid_t salida_pid = salida.run();
}
