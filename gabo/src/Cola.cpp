#include "Cola.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"
#include "defines.h"
#include "Logger.h"
#include "SignalHandler.h"
#include "SIG_Trap.h"
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

    // trap de SIGINT
    SIG_Trap sigint_handler(SIGINT);
    SIG_Trap sig_pasar_a_entrada(SIGPASARAENTRADA);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGPASARAENTRADA, &sig_pasar_a_entrada);

    Logger *l = Logger::getInstance();

    l->Log("COLA", "Creo canal de lectura a personas", DEBUG);
    FifoLectura canal_escuchar_de_personas ( nombre + C_PERSONAS_A_COLA );

    l->Log("COLA", "Creo canal de escritura a entrada", DEBUG);
    FifoEscritura canal_escribir_a_entrada( nombre + C_COLA_A_ENTRADA );

    pid_t pid_leido;
    while(sigint_handler.signalWasReceived() == 0){ // ver cuando frenar (para que cierre bien y se cierren los pipes)
        canal_escuchar_de_personas.abrir(); /// ver si en el prox paso deje de leer pq se acabo la gente o se lleno la capacidad
        // basicamente leo mientras haya gente queriendo entrar y no necesito pasarle gente a la entrada
        // si la entrada necesita gente me ocupo de eso primero
        // esto es para que la "cola" se mantenga en memoria y no sea el fifo en si
        // porque si hay muchisima gente se podria llenar el fifo y ahi se pierde el orden de llegada
        while(/*!(personas.size() > 0 && sig_pasar_a_entrada.signalWasReceived()) &&*/ canal_escuchar_de_personas.leer(static_cast<void*>(&pid_leido),sizeof(pid_t)) > 0){
            personas.push(pid_leido);
            std::string intermedio = "Entra la persona " + std::to_string(pid_leido) + " a la cola";
            l->Log("COLA", intermedio, DEBUG);
        }
        canal_escuchar_de_personas.cerrar();

        // si frene porque tenia que pasarle gente a la entrada, hago eso
        if(sig_pasar_a_entrada.signalWasReceived() && personas.size() > 0){
            pid_t prox = personas.front();
            canal_escribir_a_entrada.abrir();
            canal_escribir_a_entrada.escribir(static_cast<const void*>(&prox),sizeof(prox));
            canal_escribir_a_entrada.cerrar();
            personas.pop();
            sig_pasar_a_entrada.reset();

            std::string intermedio = "Paso la persona " + std::to_string(prox) + " a la entrada";
            l->Log("COLA", intermedio, DEBUG);
        }
    }

    l->Log("COLA", "Se cierra la cola", DEBUG);
    canal_escribir_a_entrada.cerrar();
    canal_escuchar_de_personas.cerrar();
    canal_escuchar_de_personas.eliminar();
}
