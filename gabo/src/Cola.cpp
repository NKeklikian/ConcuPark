#include "Cola.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"
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

    Logger *l = Logger::getInstance();

    l->Log("COLA", "Creo canal de lectura a personas", DEBUG);
    FifoLectura canal_escuchar_de_personas ( nombre + C_PERSONAS_A_COLA );

    l->Log("COLA", "Creo canal de escritura a entrada", DEBUG);
    FifoEscritura canal_escribir_a_entrada( nombre + C_COLA_A_ENTRADA );

    l->Log("COLA", "Abro canal a entrada", DEBUG);
    canal_escribir_a_entrada.abrir();

    pid_t pid_leido;
    while(true){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_escuchar_de_personas.abrir();
        while(canal_escuchar_de_personas.leer(static_cast<void*>(&pid_leido),sizeof(pid_t))){
            //personas.push_back(pid_leido);
            std::string intermedio = "Entra la persona " + std::to_string(pid_leido) + " a la cola";
            l->Log("COLA", intermedio, DEBUG);

            canal_escribir_a_entrada.escribir(static_cast<const void*>(&pid_leido),sizeof(pid_leido));
        }
        canal_escuchar_de_personas.cerrar();
    }

    canal_escribir_a_entrada.cerrar();
    canal_escuchar_de_personas.cerrar();
    canal_escuchar_de_personas.eliminar();
}
