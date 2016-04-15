#include "Persona.h"
#include "Logger.h"
#include "defines.h"
#include "SignalHandler.h"
#include "SIG_Trap.h"
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

void Persona::_run(){

    // estan definidas en defines.h, usa SIGUSR1 que solo lo manda el usuario
    SIG_Trap sig_pagar(SIGPAGAR);
    SIG_Trap sig_salir(SIGBAJARSE);
    SignalHandler::getInstance()->registrarHandler(SIGPAGAR, &sig_pagar);
    SignalHandler::getInstance()->registrarHandler(SIGBAJARSE, &sig_salir);

    Logger* logger = Logger::getInstance();

    std::random_device rd;
    while(plata > 0){
        std::string intermedio = "Tengo " + std::to_string(plata) + " pesos";
        logger->Log("PERSONA", intermedio, DEBUG);

        // voy a un juego random de la lista que me dio el parque
        int chosen_game = rd()%(nom_juegos.size()-0) + 0;
        // el nombre lo saco del vector
        std::string nombre = nom_juegos[chosen_game];
        FifoEscritura canal_a_cola ( nombre + C_PERSONAS_A_COLA ); // la idea es 'nombre juego' + 'que fifo quiero'

        // me anoto en la cola con mi pid
        canal_a_cola.abrir();
        pid_t pid = getpid();
        canal_a_cola.escribir ( static_cast<const void*>(&pid),sizeof(pid) );
        logger->Log("PERSONA", "Me puse en la cola del juego", DEBUG);
        canal_a_cola.cerrar();

        /// TODO: pausar hasta que me cobren (REVISAR)
        // esto de mandar una senial para decirle cuando usar el fifo creo que no era lindo
        // tal vez deberia cambiarse por un semaforo
         while(!sig_pagar.signalWasReceived()){
            pause(); // cambiar pause por sigsuspend?
        }
        sig_pagar.reset();
        int pagar = costo_juegos[chosen_game];
        FifoEscritura canal_a_entrada( nombre + C_COBRAR_A_PERSONA);
        canal_a_entrada.abrir();
        canal_a_entrada.escribir(static_cast<const void*>(&pagar),sizeof(pagar)); // ver bytes escritos
        canal_a_entrada.cerrar();
        plata -= pagar;
        intermedio = "Pago " + std::to_string(pagar) + " pesos";
        logger->Log("PERSONA", intermedio, DEBUG);

        // espero a que me notifiquen que termino el juego y puedo salir
        while(!sig_salir.signalWasReceived()){
            pause(); // cambiar pause por sigsuspend?
        }
        sig_salir.reset();
        logger->Log("PERSONA", "Me voy del juego", DEBUG);
    }
    std::string intermedio = "Tengo " + std::to_string(plata) + " pesos";
    logger->Log("PERSONA", intermedio, DEBUG);
}
