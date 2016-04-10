#include "Runnable.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

Runnable::Runnable()
{
    pid_t pid = fork();
    child = (pid == 0);
    if(pid == -1){
        child = false;
        perror("Runnable error: ");
    }
    // se puede hacer que directo no haya run publica, y se llama a _run en el constructor
    // entonces en el padre solo hay que crear el objeto
}

Runnable::~Runnable()
{
    //dtor
}

void Runnable::run(){
    if(child){
        _run();
        exit(0);
    }
}
