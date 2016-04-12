#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include "sys/signal.h"

// C es canal
#define C_PERSONAS_A_COLA "_personas_a_cola"
#define C_PERSONAS_A_ENTRADA "_personas_a_entrada"
#define C_COLA_A_ENTRADA "_cola_a_entrada"
#define C_LOGGER "/tmp/logger"
#define C_SALIDA "_salida"

// seniales
#define SIGPAGAR SIGUSR1
#define SIGBAJARSE SIGUSR2
#define SIGPASARAENTRADA SIGUSR1

#endif // DEFINES_H_INCLUDED
