#include "Administrador.h"
#include "SIG_Trap.h"
#include "SignalHandler.h"
#include "Logger.h"
#include "Caja.h"

Administrador::Administrador(int periodo_chequeo) : periodo_chequeo(periodo_chequeo)
{
    //ctor
}

Administrador::~Administrador()
{
    //dtor
}

void Administrador::_run(){

    // los traps no los pongo en el init porque corre en el padre
    SIG_Trap sigint_handler(SIGINT);
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    Logger::log("ADMIN", "Empiezo el administrador", DEBUG);

    while(!sigint_handler.signalWasReceived()){

        Caja* caja = Caja::getInstance();
        int plata = caja->getPlata();

        Logger::log("ADMIN", "Hay " + std::to_string(plata) + " en la caja", DEBUG);

        sleep(periodo_chequeo);
    }

    SignalHandler::destruir();
}
