#include <iostream>
#include "Logger.h"
#include "Cola.h"
#include "Entrada.h"
#include "Salida.h"

using namespace std;

int main()
{
    Logger* logger = Logger::getInstance();
    logger->run();
    logger->Log("MAIN", "hola", WARNING);

    Cola cola("/tmp/juego");
    cola.run();

    Entrada entrada("/tmp/juego", 5);
    entrada.run();

    Salida salida("/tmp/juego");
    salida.run();

    return 0;
}
