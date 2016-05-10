#include "Persona.h"
#include "Logger.h"
#include "defines.h"
#include "SignalHandler.h"
#include "SIG_Trap.h"
#include "Semaforo.h"
#include <random>

Persona::Persona(int p) : plata(p)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

void Persona::loadInfoParque(std::vector<std::string> &n_juegos, std::vector<int> &c_juegos){
    for(int i = 0; i < n_juegos.size(); ++i){
        nom_juegos.push_back(n_juegos[i]);
        costo_juegos.push_back(c_juegos[i]);
    }
}

void Persona::init(){
}

void Persona::_run(){

    Logger* logger = Logger::getInstance();

    std::random_device rd;
    while(plata > 0){
        Logger::log("PERSONA", "Tengo " + std::to_string(plata) + " pesos", DEBUG);

        // voy a un juego random de la lista que me dio el parque
        // si no lo puedo pagar, lo saco de mi lista de juegos para ir
        // si no quedan juegos, me voy del parque
        int chosen_game = 0;

        while(nom_juegos.size() != 0){
            chosen_game = rd()%(nom_juegos.size()-0) + 0;
            if(plata >= costo_juegos[chosen_game]){
                break;
            } else {
                nom_juegos.erase(nom_juegos.begin() + chosen_game);
                costo_juegos.erase(costo_juegos.begin() + chosen_game);
            }
        }

        if(nom_juegos.size() == 0){
            break;
        }

        // el nombre lo saco del vector
        std::string nombre = nom_juegos[chosen_game];

        Semaforo sem_entrada(nombre, SEM_ENTRADA, 0);
        sem_entrada.get();
        Semaforo sem_cobrar(nombre, SEM_COBRAR, 0);
        sem_cobrar.get();
        Semaforo sem_salida(nombre, SEM_SALIDA, 0);
        sem_salida.get();
        Semaforo sem_salir(nombre, SEM_SALIR, 0);
        sem_salir.get();

        // aviso a la entrada que llegue
        sem_entrada.v();
        Logger::log("PERSONA", "Me pongo en la entrada del juego", DEBUG);
        // espero a que me cobren
        Logger::log("PERSONA", "Espero a que me cobren", DEBUG);
        sem_cobrar.p();
        int pagar = costo_juegos[chosen_game];
        plata -= pagar;
        //intermedio = "Pago " + std::to_string(pagar) + " pesos al juego " + nombre;
        Logger::log("PERSONA", "Pago " + std::to_string(pagar) + " pesos al juego " + nombre, DEBUG);
        // espero a que termine el juego
        Logger::log("PERSONA", "Espero a que termine el juego", DEBUG);
        sem_salir.p();
        Logger::log("PERSONA", "Paso a la salida", DEBUG);
        // aviso a la salida que me fui
        Logger::log("PERSONA", "Me voy del juego " + nombre, DEBUG);
        sem_salida.v();
    }
    std::string intermedio = "Me voy del parque, me quedaron " + std::to_string(plata) + " pesos";
    Logger::log("PERSONA", intermedio, DEBUG);
}
