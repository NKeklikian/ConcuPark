#include "Salida.h"
#include "../../fifos/FifoLectura.h"
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
    //std::ofstream logger;
    //logger.open("salida_log.txt", std::ofstream::out);
    Logger *l = Logger::getInstance();

    // esto tal vez va en la clase como variables
    static const int BUFFSIZE = sizeof(getpid());
    //logger << "Creo canal de lectura a entrada" << std::endl;
    l->Log("SALIDA", "Creo canal de lectura a entrada", DEBUG);
    FifoLectura canal_escuchar_de_entrada ( nombre + C_SALIDA );
    //logger << "Abro canal a entrada" << std::endl;
    l->Log("SALIDA", "Abro canal a entrada", DEBUG);

    pid_t pid_leido;
    while(true){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_escuchar_de_entrada.abrir();
        while(canal_escuchar_de_entrada.leer(static_cast<void*>(&pid_leido),BUFFSIZE)){
            //personas.push_back(pid_leido);
            //logger << "Sale la persona " << pid_leido << std::endl;
            std::string intermedio = "Sale la persona " + std::to_string(pid_leido);
            l->Log("SALIDA", intermedio, DEBUG);

            /// enviar al pid senial para que se desbloquee y siga andando por el parque
        }
        canal_escuchar_de_entrada.cerrar();
    }

    //logger.close();
    canal_escuchar_de_entrada.cerrar();
    canal_escuchar_de_entrada.eliminar();
}
