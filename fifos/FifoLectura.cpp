#include "FifoLectura.h"
#include <stdio.h>

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
	if(fd == -1){
        perror("FifoLectura error: ");
        // que onda? se cierra el programa?
    }
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {
	ssize_t bytes_read = read ( fd,buffer,buffsize );
	if(bytes_read == -1){
        perror("FifoLectura error: ");
    }
	return bytes_read;
}
