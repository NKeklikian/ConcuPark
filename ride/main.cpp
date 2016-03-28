#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define NAME "/semtest"

using namespace std;

// CAMBIAR signal POR sigaction

void sigproc(int){
    signal(SIGINT, sigproc);
    cout << "borrando el semaforo" << endl;

    int status = sem_unlink(NAME);
    if(status == -1){
        perror("sem_unlink: sem_unlink failed");
    }
    exit(0);
}

void sigusr1(int){
}

int main()
{
    // seteo memoria compartida de tamanio 1 int para que me pasen la plata


    // seteo el semaforo

    signal(SIGINT, sigproc);

    sem_t* sem = sem_open(NAME, O_CREAT, ALLPERMS, 1);
    if(sem == SEM_FAILED){
        perror("sem_open: sem_open failed");
        exit(1);
    }

    while(true);

    int status = sem_unlink(NAME);
    if(status == -1){
        perror("sem_unlink: sem_unlink failed");
    }

    return 0;
}
