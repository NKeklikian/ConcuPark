#include "Caja.h"
#include "defines.h"
#include <fcntl.h>
#include <unistd.h>

static Caja* instance = 0;

Caja::Caja() : plata("caja", 4), sem_caja("caja", SEM_CAJA, 1)
{

}

int Caja::crear(){
    // creo archivo temporal para la caja para ftok
    close ( open ( "caja",O_CREAT,0777 ) );
    plata.crear();
    sem_caja.crear();
    return 1; // cambiar y ver errores
}

void Caja::eliminar(){
    plata.liberar();
    sem_caja.eliminar();
    unlink("caja");
}

Caja::~Caja()
{
    if(instancia != NULL)
        delete instancia;
    instancia = NULL;
}

Caja* Caja::getInstance(){
    if(!instance){
        instance = new Caja;
    }
    return instance;
}

int Caja::getPlata(){
    // espero a que este libre la caja
    sem_caja.p();

    int plata_leida = plata.leer();

    // libero la caja
    sem_caja.v();

    return plata_leida;
}

void Caja::addPlata(int cant){
    // espero a que este libre la caja
    sem_caja.p();

    int plata_leida = plata.leer();
    plata.escribir(plata_leida + cant);

    // libero la caja
    sem_caja.v();
}
