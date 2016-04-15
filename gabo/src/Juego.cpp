#include "Juego.h"
#include "Cola.h"
#include "Entrada.h"
#include "Salida.h"
#include "SIG_Trap.h"
#include "SignalHandler.h"
#include "Logger.h"

Juego::Juego(std::string n, int cap) : nombre(n), capacidad(cap)
{
    //ctor
}

Juego::~Juego()
{
    //dtor
}

void Juego::_run(){
    Logger* l = Logger::getInstance();

    l->Log("JUEGO", "Creo la cola", DEBUG);
    Cola cola(nombre);
    pid_t cola_pid = cola.run();

    l->Log("JUEGO", "Creo la entrada", DEBUG);
    Entrada entrada(nombre, capacidad, cola_pid);
    pid_t entrada_pid = entrada.run();

    l->Log("JUEGO", "Creo la salida", DEBUG);
    Salida salida(nombre);
    pid_t salida_pid = salida.run();

    // esto esta para poder cerrar todo el juego matando a un solo proceso, pero se podria manejar de
    // alguna otra forma donde no sea necesario mantener un proceso al pedo (aunque esta bloqueado asi que no molesta)
    SIG_Trap sig_int(SIGINT);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sig_int);

    while(!sig_int.signalWasReceived()){
        pause();
    }

    if(sig_int.signalWasReceived()){
        l->Log("JUEGO", "Cierro el juego y todas sus estructuras", DEBUG);
        kill(cola_pid, SIGINT);
        kill(entrada_pid, SIGINT);
        kill(salida_pid, SIGINT);
    } else {
        // nunca deberia llegar aca
    }
}
