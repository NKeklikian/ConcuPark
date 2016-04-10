#include <iostream>
#include <vector>
//#include "../fifos/FifoEscritura.h"
#include "../fifos/FifoLectura.h"

using namespace std;

int main()
{
    static const int BUFFSIZE = sizeof(getpid());
	static const std::string ARCHIVO_FIFO = "/tmp/cola_juego";

    FifoLectura canal ( ARCHIVO_FIFO );
    char buffer[BUFFSIZE];
    pid_t pid;

    canal.abrir();

    vector<pid_t> personas;
    int capacidad_max = 10;

    while(personas.size() < capacidad_max){
        std::cout << "[Lector] A punto de leer del fifo" << std::endl;
        ssize_t bytesLeidos = canal.leer(static_cast<void*>(&pid),BUFFSIZE);
        if(bytesLeidos == 0){
            canal.cerrar();
            canal.abrir();
            continue;
        }

        //std::string mensaje = buffer;
        //mensaje.resize ( bytesLeidos );
        //cout << mensaje << endl;
        //pid_t pid_leido = std::stoi(mensaje, nullptr, 10);
        std::cout << "[Lector] Lei el pid del fifo: " << pid << std::endl;
        personas.push_back(pid);
        cout << "[Lector] Estado actual de la cola: ";
        for(int i = 0; i < personas.size(); ++i){
            cout << personas[i] << " ";
        }
        cout << endl;
    }
    canal.cerrar();
    canal.eliminar();

    cout << "[Lector] La cola esta llena" << endl;
    std::cout << "[Lector] Fin del proceso" << std::endl;
    exit ( 0 );

    return 0;
}
