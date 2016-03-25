#include "Parque.h"

Parque::Parque(){
    juegos.push_back(new Juego(3, 1));
}

Parque::~Parque(){
    while(!juegos.empty()){
        delete juegos.back();
        juegos.pop_back();
    }
}
