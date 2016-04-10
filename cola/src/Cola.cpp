#include "Cola.h"
#include "../../fifos/FifoLectura.h"
#include <iostream>

Cola::Cola(std::string nom) : nombre(nom)
{
    // setear parametros como largo maximo de la cola,
    // y lo que haga falta para comunicarse con el juego al que pertenece
}

Cola::~Cola()
{
    //dtor
}

void Cola::_run(){
    // esto tal vez va en la clase como variables
    static const int BUFFSIZE = sizeof(getpid());
    FifoLectura canal ( nombre );

    pid_t pid_leido;
    while(true){
        canal.abrir();
        while(canal.leer(static_cast<void*>(&pid_leido),BUFFSIZE)){
            personas.push_back(pid_leido);
            /*std::cout << "[Cola] Estado actual de la cola: ";
            for(int i = 0; i < personas.size(); ++i){
                std::cout << personas[i] << " ";
            }
            std::cout << std::endl;*/
        }
        canal.cerrar();
    }
    canal.eliminar();
}
