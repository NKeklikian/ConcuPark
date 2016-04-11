#include "Persona.h"
#include "Logger.h"
#include "defines.h"
#include "SignalHandler.h"

Persona::Persona(int p) : plata(p)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

/// por ahora le puse esto solo para que se bloquee hasta que le llega la senial.
/// cuando haga cosas distintas en funcion a que senial le llega, deberia tener handlers distintos
/// o van los dos aca con un if cabeza, total son solo dos seniales posibles
int Persona::handleSignal ( int signum ) {
    //assert ( signum == SIGPAGAR );
    //this->gracefulQuit = 1;
    return 0;
}

void Persona::_run(){
    Logger* logger = Logger::getInstance();

    // estan definidas en defines.h, usa SIGUSR1 y SIGUSR2 que solo los manda el usuario
    SignalHandler::getInstance()->registrarHandler(SIGPAGAR, this);
    SignalHandler::getInstance()->registrarHandler(SIGBAJARSE, this);

    // por ahora no loopea la persona porque solo hay un juego
    //while(true){
        // hardcodeado porque solo hay un juego
        std::string nombre = "/tmp/juego";
        FifoEscritura canal_a_cola ( nombre + C_PERSONAS_A_COLA ); // la idea es 'nombre juego' + 'que fifo quiero'
        canal_a_cola.abrir();

        // me anoto en la cola con mi pid
        pid_t pid = getpid();
        canal_a_cola.escribir ( static_cast<const void*>(&pid),sizeof(pid) );
        logger->Log("PERSONA", "Me puse en la cola del juego", DEBUG);
        canal_a_cola.cerrar ();

        /// TODO: pausar hasta que me cobren
        pause();
        logger->Log("PERSONA", "Pago tanta plata", DEBUG);

        /// TODO: pausar hasta que termine el juego
        pause();
        logger->Log("PERSONA", "Me voy del juego", DEBUG);
    //}
}
