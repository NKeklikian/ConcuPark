#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include "Juego.h"
#include "Persona.h"
#include "Semaforo.h"
#include "Logger.h"
#include "Caja.h"
#include "Administrador.h"

using namespace std;

// esto vendria a ser el parque, despues se podria abstraer a otra clase runnable
int main()
{
    Logger::log("MAIN", "Empezando simulacion", DEBUG);

    /// ---------------------- formato de archivo de configuracion ----------------------

    /*

    periodo_chequeo_caja
    cant_juegos
    costo_juego_1 capacidad_juego_1
    costo_juego_2 capacidad_juego_2
    ...
    cant_personas
    plata_persona_1
    plata_persona_2
    ...

    */

    /// ---------------------------------------------------------------------------------

    ifstream input_file;
    input_file.open("config.txt", ifstream::in);
    string temp;
    stringstream ss;
    string t;

    // leo periodo chequeo caja
    getline(input_file, temp);
    int periodo_chequeo_caja = std::stoi(temp, 0, 10);

    // leo cantidad juegos
    getline(input_file, temp);
    int cant_juegos = std::stoi(temp, 0, 10);

    // leo costo y capacidad de cada juego
    vector<int> costo_juegos;
    vector<int> capacidad_juegos;

    for(int i = 0; i < cant_juegos; ++i){
        getline(input_file, temp);
        ss << temp;

        getline(ss, t, ' ');
        costo_juegos.push_back(std::stoi(t, 0, 10));
        getline(ss, t, ' ');
        capacidad_juegos.push_back(std::stoi(t, 0, 10));
    }

    // leo cantidad personas
    getline(input_file, temp);
    int cant_personas = std::stoi(temp, 0, 10);

    // leo plata personas
    vector<int> plata_personas;
    for(int i = 0; i < cant_personas; ++i){
        getline(input_file, temp);
        plata_personas.push_back(std::stoi(temp, 0, 10));
    }

    // creo la caja
    Logger::log("MAIN", "Creo la caja", DEBUG);
    Caja* caja = Caja::getInstance();
    caja->crear();

    // creo al administrador
    Logger::log("MAIN", "Creo al administrador", DEBUG);
    Administrador administrador(periodo_chequeo_caja);
    pid_t administrador_pid = administrador.run();

    // crear juegos
    Logger::log("MAIN", "Creo juegos", DEBUG);
    //int cant_juegos = 1; /// HARDCODEO
    vector<pid_t> juegos;
    vector<string> nom_juegos;
    for(int i = 1; i <= cant_juegos; ++i){
        std::string nom_juego = "juego" + std::to_string(i);
        nom_juegos.push_back(nom_juego);

        Juego juego(nom_juego, capacidad_juegos[i-1], costo_juegos[i-1]);
        pid_t juego_pid = juego.run();
        juegos.push_back(juego_pid);
    }

    // crear personas
    Logger::log("MAIN", "Creo personas", DEBUG);
    vector<pid_t> personas;
    for(int i = 0; i < cant_personas; ++i){
        Persona persona(plata_personas[i]);
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

    // cierro los juegos
    Logger::log("MAIN", "Cierro los juegos", DEBUG);
    while(juegos.size() > 0){
        kill(juegos[juegos.size()-1], SIGINT);
        waitpid(juegos[juegos.size()-1], &status, 0);
        juegos.pop_back();
    }

    // cierro al administrador
    Logger::log("MAIN", "Cierro al administrador", DEBUG);
    kill(administrador_pid, SIGINT);
    waitpid(administrador_pid, &status, 0);

    // cierro la caja
    Logger::log("MAIN", "Cierro la caja", DEBUG);
    caja->eliminar();

    Logger::log("MAIN", "Simulacion terminada", DEBUG);
    return 0;
}
