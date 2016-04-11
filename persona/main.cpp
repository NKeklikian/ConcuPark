#include <iostream>
#include "../fifos/FifoEscritura.h"

using namespace std;

int main()
{
    static const int BUFFSIZE = sizeof(getpid());
	static const std::string ARCHIVO_FIFO = "/tmp/juego_personas_a_cola";

    FifoEscritura canal ( ARCHIVO_FIFO );

    canal.abrir();
    //sleep ( 5 );
    pid_t pid = getpid();
    string buffer = std::to_string(pid);
    //canal.escribir ( static_cast<const void*>(buffer.c_str()),buffer.length() );
    canal.escribir ( static_cast<const void*>(&pid),sizeof(pid) );
    std::cout << "[Escritor] Escribi el mensaje " << pid << " en el fifo" << std::endl;


    canal.cerrar ();
    //canal.eliminar ();

    std::cout << "[Escritor] Fin del proceso" << std::endl;

    return 0;
}
