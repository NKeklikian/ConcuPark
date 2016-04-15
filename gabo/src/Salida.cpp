#include "Salida.h"
#include "FifoLectura.h"
#include "defines.h"
#include "Logger.h"
#include "SIG_Trap.h"
#include "SignalHandler.h"
#include <iostream>
#include <fstream>

Salida::Salida(std::string nom) : nombre(nom)
{
    //ctor
}

Salida::~Salida()
{
    //dtor
}

void Salida::_run(){
    // trap de SIGINT
    SIG_Trap sigint_handler(SIGINT);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    Logger *l = Logger::getInstance();

    l->Log("SALIDA", "Creo canal de lectura a entrada", DEBUG);
    FifoLectura canal_escuchar_de_entrada ( nombre + C_SALIDA );

    pid_t pid_leido;
    while(sigint_handler.signalWasReceived() == 0){
        canal_escuchar_de_entrada.abrir();
        while(canal_escuchar_de_entrada.leer(static_cast<void*>(&pid_leido),sizeof(pid_t)) > 0){
            std::string intermedio = "Sale la persona " + std::to_string(pid_leido);
            l->Log("SALIDA", intermedio, DEBUG);
            // enviar al pid senial para que se desbloquee y siga andando por el parque
            kill(pid_leido, SIGBAJARSE);
        }
        canal_escuchar_de_entrada.cerrar();
    }

    l->Log("SALIDA", "Se cierra la salida", DEBUG);
    canal_escuchar_de_entrada.cerrar();
    canal_escuchar_de_entrada.eliminar();
}
