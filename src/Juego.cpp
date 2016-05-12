#include "Juego.h"
#include "SIG_Trap.h"
#include "SignalHandler.h"
#include "Logger.h"
#include "defines.h"
#include "Caja.h"

int Juego::cant = 0;

Juego::Juego(std::string n, int cap, int costo) : nombre(n), capacidad(cap), costo(costo),
                                        sem_entrada(this->nombre, SEM_ENTRADA, 0),
                                        sem_entrada_libre(this->nombre, SEM_ENTRADA_LIBRE, 0),
                                        sem_cobrar(this->nombre, SEM_COBRAR, 0),
                                        sem_salida(this->nombre, SEM_SALIDA, 0),
                                        sem_salir(this->nombre, SEM_SALIR, 0)
{
    LOG_JUEGO = "JUEGO" + std::to_string(++cant);
}

Juego::~Juego()
{
    //dtor
}

void Juego::init(){
    Logger::log(LOG_JUEGO, "Creando estructuras del juego", DEBUG);

    // cada juego crea un archivo temporal que lo identifica (para la creacion de semaforos)
    close ( open ( this->nombre.c_str(),O_CREAT,0777 ) );

    sem_entrada.crear();
    sem_entrada_libre.crear();
    sem_cobrar.crear();
    sem_salida.crear();
    sem_salir.crear();
}

void Juego::_run(){

    // los traps no los pongo en el init porque corre en el padre
    SIG_Trap sigint_handler(SIGINT);
    SIG_Trap sigalrm_handler(SIGALRM);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);
    SignalHandler::getInstance()->registrarHandler(SIGALRM, &sigalrm_handler);

    int gente = 0;
    while(!sigint_handler.signalWasReceived()){
        while(!sigint_handler.signalWasReceived() && gente < capacidad && !sigalrm_handler.signalWasReceived()){
            // espero a que llegue alguien
            Logger::log(LOG_JUEGO, "Espero a que llegue alguien", DEBUG);
            if(sem_entrada.p() != -1){ // si da -1 es pq se interrumpio la espera por la alarma
                // hago pasar a uno para cobrarle
                Logger::log(LOG_JUEGO, "Hago pasar una persona para cobrarle", DEBUG);
                sem_cobrar.v();
                ++gente;

                Logger::log(LOG_JUEGO, "Guardo " + std::to_string(costo) + " pesos en la caja", DEBUG);
                Caja* caja = Caja::getInstance();
                caja->addPlata(costo);
                //Logger::log("JUEGO", "La caja ahora tiene " + std::to_string(caja->getPlata()) + " pesos", DEBUG);

                Logger::log(LOG_JUEGO, "Espero a que entre la persona", DEBUG);
                sem_entrada_libre.p();

                // desde que llega el primero empiezo el timer
                if(gente == 1){
                    Logger::log(LOG_JUEGO, "Llego la 1era persona, si no se llena en 5 segs arranco", DEBUG);
                    alarm(5); /// hardcodeo de tiempo de espera
                }
            }
        }

        if(!sigint_handler.signalWasReceived()){
            if(sigalrm_handler.signalWasReceived()){
                Logger::log(LOG_JUEGO, "Se acabo el tiempo de espera", DEBUG);
                sigalrm_handler.reset();
            } else if(gente == capacidad){
                Logger::log(LOG_JUEGO, "Se lleno el juego", DEBUG);
            }
            alarm(0); // apago la alarma
            Logger::log(LOG_JUEGO, "Arranca el juego con " + std::to_string(gente) + " personas", DEBUG);

            // simulo la duracion del juego
            sleep(10);
            Logger::log(LOG_JUEGO, "Salen todos del juego", DEBUG);
        }

        while(!sigint_handler.signalWasReceived() && gente > 0){
            // aviso a una persona que puede salir
            Logger::log(LOG_JUEGO, "Dejo salir a una persona", DEBUG);
            sem_salir.v();
            // espero a que salga
            Logger::log(LOG_JUEGO, "Espero a que salga", DEBUG);
            sem_salida.p();
            --gente;
        }
    }

    Logger::log(LOG_JUEGO, "Cierro el juego y todas sus estructuras", DEBUG);
    sem_entrada.eliminar();
    sem_entrada_libre.eliminar();
    sem_cobrar.eliminar();
    sem_salida.eliminar();
    sem_salir.eliminar();
    unlink(this->nombre.c_str());
    SignalHandler::destruir();
}
