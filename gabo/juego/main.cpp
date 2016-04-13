#include <iostream>
#include "Logger.h"
#include "Juego.h"

using namespace std;

int main()
{
    Logger* logger = Logger::getInstance();
    logger->run();
    logger->Log("MAIN", "hola", WARNING);

    Juego juego("/tmp/juego", 1);
    juego.run();

    return 0;
}
