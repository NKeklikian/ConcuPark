#include "Entrada.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"
#include "defines.h"
#include "Logger.h"
#include "SIG_Trap.h"
#include "SignalHandler.h"
#include <time.h>
#include <fstream>

Entrada::Entrada(std::string nom, int cap, pid_t c) : nombre(nom), capacidad(cap), cola_pid(c)
{
    //ctor
}

Entrada::~Entrada()
{
    //dtor
}

void Entrada::_run(){

    // trap de SIGINT
    SIG_Trap sigint_handler(SIGINT);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    Logger *l = Logger::getInstance();

    l->Log("ENTRADA", "Creo canal de lectura a cola", DEBUG);
    FifoLectura canal_escuchar_de_cola ( nombre + C_COLA_A_ENTRADA );

    l->Log("ENTRADA", "Creo canal de escritura a salida", DEBUG);
    FifoEscritura canal_a_salida(nombre + C_SALIDA);

    while(sigint_handler.signalWasReceived() == 0){
        pid_t pid_leido;

        /// solo arranco si estoy lleno de gente (habria que poner un timer)
        while(sigint_handler.signalWasReceived() == 0 && personas.size() < capacidad){
            //l->Log("ENTRADA", "Abro canal de lectura a cola", DEBUG);
            kill(cola_pid, SIGPASARAENTRADA);
            canal_escuchar_de_cola.abrir();
            ssize_t bytes_leidos = canal_escuchar_de_cola.leer(static_cast<void*>(&pid_leido),sizeof(pid_t));
            if(bytes_leidos > 0){

                /// TODO: le cobro a la persona (aca deberia esperar a que me responda que pago)
                kill(pid_leido, SIGPAGAR);

                // la meto al juego
                personas.push_back(pid_leido);

                std::string intermedio = "Entra al juego: " + std::to_string(pid_leido);
                l->Log("ENTRADA", intermedio, DEBUG);
            }
            canal_escuchar_de_cola.cerrar();
        }

        /// TODO: o me llene de gente o paso el tiempo, va a empezar el juego
        if(personas.size() >= capacidad){
            l->Log("ENTRADA", "Se lleno el juego", DEBUG);

            std::string intermedio = "Arranca el juego con: ";
            for(int i = 0; i < personas.size(); ++i){
                intermedio += std::to_string(personas[i]) + " ";
            }
            l->Log("ENTRADA", intermedio, DEBUG);
            // simulo la duracion del juego
            sleep(10);

            // notificar a la salida con un pipe que se va toda la gente
            l->Log("ENTRADA", "Salen todos del juego", DEBUG);
            canal_a_salida.abrir();
            for(int i = 0; i < personas.size(); ++i){
                pid_t sale = personas[i];
                canal_a_salida.escribir(static_cast<void*>(&sale), sizeof(sale));
            }
            canal_a_salida.cerrar();
            personas.clear();
        }
    }

    l->Log("ENTRADA", "Se cierra la entrada", DEBUG);
    canal_a_salida.cerrar();
    canal_escuchar_de_cola.cerrar();
    canal_escuchar_de_cola.eliminar();
}
