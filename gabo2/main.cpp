#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
//#include <stdlib.h>
#include "Juego.h"
#include "Persona.h"
#include "Semaforo.h"
#include "Logger.h"

using namespace std;

// esto vendria a ser el parque, despues se podria abstraer a otra clase runnable
int main()
{
    Logger::log("MAIN", "Empezando simulacion", DEBUG);

    // crear juegos
    Logger::log("MAIN", "Creo juegos", DEBUG);
    int cant_juegos = 1; /// HARDCODEO
    vector<pid_t> juegos;
    vector<string> nom_juegos;
    vector<int> costo_juegos;
    for(int i = 1; i <= cant_juegos; ++i){
        std::string nom_juego = "juego" + std::to_string(i);
        nom_juegos.push_back(nom_juego);
        costo_juegos.push_back(5); /// costo esta hardcodeado aca

        Juego juego(nom_juego, 2); ///capacidad esta hardcodeada aca
        pid_t juego_pid = juego.run();
        juegos.push_back(juego_pid);
    }

    // crear personas
    Logger::log("MAIN", "Creo personas", DEBUG);
    int cant_personas = 2; /// HARDCODEO
    vector<pid_t> personas;
    for(int i = 0; i < cant_personas; ++i){
        Persona persona(5); /// aca esta hardcodeada la plata
        persona.loadInfoParque(nom_juegos, costo_juegos);

        pid_t persona_pid = persona.run();
        personas.push_back(persona_pid);
    }

    // espero a que se vayan todas las personas
    int status;
    while(personas.size() > 0){
        waitpid(personas[personas.size()-1], &status, 0);
        personas.pop_back();
    }
    Logger::log("MAIN", "Se fueron todos", DEBUG);

    // cierro todo
    Logger::log("MAIN", "Cierro los juegos", DEBUG);
    while(juegos.size() > 0){
        kill(juegos[juegos.size()-1], SIGINT);
        waitpid(juegos[juegos.size()-1], &status, 0);
        juegos.pop_back();
    }

    Logger::log("MAIN", "Simulacion terminada", DEBUG);
    return 0;
}
