#include "Salida.h"
#include "FifoLectura.h"
#include "defines.h"
#include "Logger.h"
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
    Logger *l = Logger::getInstance();

    l->Log("SALIDA", "Creo canal de lectura a entrada", DEBUG);
    FifoLectura canal_escuchar_de_entrada ( nombre + C_SALIDA );

    pid_t pid_leido;
    while(true){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_escuchar_de_entrada.abrir();
        while(canal_escuchar_de_entrada.leer(static_cast<void*>(&pid_leido),sizeof(pid_t))){
            //personas.push_back(pid_leido);
            std::string intermedio = "Sale la persona " + std::to_string(pid_leido);
            l->Log("SALIDA", intermedio, DEBUG);

            // enviar al pid senial para que se desbloquee y siga andando por el parque
            kill(pid_leido, SIGBAJARSE);
        }
        canal_escuchar_de_entrada.cerrar();
    }

    canal_escuchar_de_entrada.cerrar();
    canal_escuchar_de_entrada.eliminar();
}
