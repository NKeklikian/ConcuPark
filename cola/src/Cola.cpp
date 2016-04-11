#include "Cola.h"
#include "../../fifos/FifoLectura.h"
#include "../../fifos/FifoEscritura.h"
#include "defines.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>

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
    //std::ofstream logger;
    //logger.open("cola_log.txt", std::ofstream::out);
    Logger *l = Logger::getInstance();

    // esto tal vez va en la clase como variables
    static const int BUFFSIZE = sizeof(getpid());
    l->Log("COLA", "Creo canal de lectura a personas", DEBUG);
    //logger << "Creo canal de lectura a personas" << std::endl;
    FifoLectura canal_escuchar_de_personas ( nombre + C_PERSONAS_A_COLA );
    //logger << "Creo canal de escritura a entrada" << std::endl;
    l->Log("COLA", "Creo canal de escritura a entrada", DEBUG);
    FifoEscritura canal_escribir_a_entrada( nombre + C_COLA_A_ENTRADA );
    //logger << "Abro canal a entrada" << std::endl;
    l->Log("COLA", "Abro canal a entrada", DEBUG);
    canal_escribir_a_entrada.abrir();

    pid_t pid_leido;
    while(true){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_escuchar_de_personas.abrir();
        while(canal_escuchar_de_personas.leer(static_cast<void*>(&pid_leido),BUFFSIZE)){
            //personas.push_back(pid_leido);
            std::string intermedio = "Entra la persona " + std::to_string(pid_leido) + " a la cola";
            l->Log("COLA", intermedio, DEBUG);
            //logger << "Entra la persona " << pid_leido << " a la cola" << std::endl;
            canal_escribir_a_entrada.escribir(static_cast<const void*>(&pid_leido),sizeof(pid_leido));
            /*std::cout << "[Cola] Estado actual de la cola: ";
            for(int i = 0; i < personas.size(); ++i){
                std::cout << personas[i] << " ";
            }
            std::cout << std::endl;*/
        }
        canal_escuchar_de_personas.cerrar();
    }

    //logger.close();
    canal_escribir_a_entrada.cerrar();
    canal_escribir_a_entrada.eliminar();
    canal_escuchar_de_personas.eliminar();
}
