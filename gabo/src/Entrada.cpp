#include "Entrada.h"
#include "FifoLectura.h"
#include "FifoEscritura.h"
#include "defines.h"
#include "Logger.h"
#include <time.h>
#include <fstream>

Entrada::Entrada(std::string nom, int cap) : nombre(nom), capacidad(cap)
{
    //ctor
}

Entrada::~Entrada()
{
    //dtor
}

void Entrada::_run(){

    Logger *l = Logger::getInstance();

    l->Log("ENTRADA", "Creo canal de lectura a cola", DEBUG);
    FifoLectura canal_escuchar_de_cola ( nombre + C_COLA_A_ENTRADA );

    l->Log("ENTRADA", "Abro canal de lectura a cola", DEBUG);
    canal_escuchar_de_cola.abrir();

    l->Log("ENTRADA", "Creo canal de escritura a salida", DEBUG);
    FifoEscritura canal_a_salida(nombre + C_SALIDA);

    while(true){
        pid_t pid_leido;

        /// solo arranco si estoy lleno de gente (habria que poner un timer)
        while(personas.size() < capacidad){
            /// esto esta mal, deberia bloquearme hasta que llega alguien o pasa el tiempo, sino estoy tirando tiempo de cpu
            ssize_t bytes_leidos = canal_escuchar_de_cola.leer(static_cast<void*>(&pid_leido),sizeof(pid_t));
            if(bytes_leidos != 0){

                /// TODO: le cobro a la persona
                kill(pid_leido, SIGPAGAR);

                // la meto al juego
                personas.push_back(pid_leido);

                std::string intermedio = "Entra al juego: " + std::to_string(pid_leido);
                l->Log("ENTRADA", intermedio, DEBUG);
            }
        }

        /// TODO: o me llene de gente o paso el tiempo, va a empezar el juego
        if(personas.size() >= capacidad){
            l->Log("ENTRADA", "Se lleno el juego", DEBUG);
        } else {
            l->Log("ENTRADA", "Se acabo el tiempo de espera", DEBUG);
        }

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

    l->Log("ENTRADA", "Se cierra el juego", DEBUG);
    canal_a_salida.cerrar();
    canal_escuchar_de_cola.cerrar();
    canal_escuchar_de_cola.eliminar();
}
