#include "Persona.h"
#include "Logger.h"
#include "defines.h"
#include "SignalHandler.h"
#include "SIG_Trap.h"

Persona::Persona(int p) : plata(p)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

void Persona::_run(){

    // estan definidas en defines.h, usa SIGUSR1 y SIGUSR2 que solo los manda el usuario
    SIG_Trap sig_pagar(SIGPAGAR);
    SIG_Trap sig_salir(SIGBAJARSE);
    SignalHandler::getInstance()->registrarHandler(SIGPAGAR, &sig_pagar);
    SignalHandler::getInstance()->registrarHandler(SIGBAJARSE, &sig_salir);

    Logger* logger = Logger::getInstance();
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
        while(!sig_pagar.signalWasReceived()){
            pause(); // cambiar pause por sigsuspend?
        }
        logger->Log("PERSONA", "Pago tanta plata", DEBUG);

        while(!sig_salir.signalWasReceived()){
            pause(); // cambiar pause por sigsuspend?
        }
        logger->Log("PERSONA", "Me voy del juego", DEBUG);
    //}
}
