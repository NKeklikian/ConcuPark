#include "Entrada.h"
#include "../../fifos/FifoLectura.h"
#include "../../fifos/FifoEscritura.h"
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
    //std::ofstream logger;
    //logger.open("entrada_log.txt", std::ofstream::out);
    Logger *l = Logger::getInstance();

    // esto tal vez va en la clase como variables
    static const int BUFFSIZE_con_cola = sizeof(getpid());
    //logger << "Creo canal de lectura a cola" << std::endl;
    l->Log("ENTRADA", "Creo canal de lectura a cola", DEBUG);
    FifoLectura canal_escuchar_de_cola ( nombre + C_COLA_A_ENTRADA );
    //logger << "Abro canal de lectura a cola" << std::endl;
    l->Log("ENTRADA", "Abro canal de lectura a cola", DEBUG);
    canal_escuchar_de_cola.abrir();

    FifoEscritura canal_a_salida(nombre + C_SALIDA);
    //logger << "Creo canal de escritura a salida" << std::endl;
    l->Log("ENTRADA", "Creo canal de escritura a salida", DEBUG);

    while(true){
        pid_t pid_leido;

        /// solo arranco si estoy lleno de gente (habria que poner un timer)
        while(personas.size() < capacidad){
            /// esto esta mal, deberia bloquearme hasta que llega alguien o pasa el tiempo, sino estoy tirando tiempo de cpu
            ssize_t bytes_leidos = canal_escuchar_de_cola.leer(static_cast<void*>(&pid_leido),BUFFSIZE_con_cola);
            if(bytes_leidos != 0){
                /// TODO: le cobro a la persona

                // la meto al juego
                personas.push_back(pid_leido);
                //logger << "Entra al juego: " << pid_leido << std::endl;
                std::string intermedio = "Entra al juego: " + std::to_string(pid_leido);
                l->Log("ENTRADA", intermedio, DEBUG);
            }
        }

        /// TODO: o me llene de gente o paso el tiempo, va a empezar el juego

        if(personas.size() >= capacidad){
            //logger << "Se lleno el juego" << std::endl;
            l->Log("ENTRADA", "Se lleno el juego", DEBUG);
        } else {
            //logger << "Se acabo el tiempo de espera" << std::endl;
            l->Log("ENTRADA", "Se acabo el tiempo de espera", DEBUG);
        }

        std::string intermedio = "Arranca el juego con: ";
        //logger << "Arranca el juego con: ";
        for(int i = 0; i < personas.size(); ++i){
            //logger << personas[i] << " ";
            intermedio += std::to_string(personas[i]) + " ";
        }
        //logger << std::endl;
        l->Log("ENTRADA", intermedio, DEBUG);
        sleep(10);

        // notificar a la salida con un pipe que se va toda la gente
        //logger << "Salen todos del juego" << std::endl;
        l->Log("ENTRADA", "Salen todos del juego", DEBUG);
        canal_a_salida.abrir();
        for(int i = 0; i < personas.size(); ++i){
            pid_t sale = personas[i];
            canal_a_salida.escribir(static_cast<void*>(&sale), sizeof(sale));
        }
        canal_a_salida.cerrar();
        personas.clear();
    }

    //logger << "Se cierra el juego" << std::endl;
    //logger.close();
    l->Log("ENTRADA", "Se cierra el juego", DEBUG);
    canal_escuchar_de_cola.cerrar();
    canal_escuchar_de_cola.eliminar();
}
