#include "Runnable.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

Runnable::Runnable()
{
    /// me gustaba mas en el constructor esto, pero no se porque se llamaba dos veces en el hijo
    /// entonces el hijo generaba otro hijo
    /*pid_t pid = fork();
    child = (pid == 0);
    if(pid == -1){
        child = false;
        perror("Runnable error: ");
    }
    std::cout << "constructor: " << getpid() << ": " << pid << std::endl;*/
    // se puede hacer que directo no haya run publica, y se llama a _run en el constructor
    // entonces en el padre solo hay que crear el objeto
}

Runnable::~Runnable()
{
    //dtor
}

pid_t Runnable::run(){
    pid_t pid = fork();
    if(pid == -1){
        child = false;
        perror("Runnable error: ");
    }

    // en el hijo corro la funcion _run y termino
    if(pid == 0){
        _run();
        exit(0);
    } else {
        return pid;
    }
    // en el padre sigo como si nada
}
