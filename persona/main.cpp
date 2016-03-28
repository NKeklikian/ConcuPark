#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>

#define NAME "/semtest"

using namespace std;

int main()
{
    pid_t pid = 0;
    for(int i = 0; i < 10; i++){
        pid = fork();
        if(pid == 0){
            break;
        }
    }

    if(pid == 0){
        sem_t* sem = sem_open(NAME, 0);
        if(sem == SEM_FAILED){
            perror("sem_open: sem_open failed");
            exit(1);
        }
        int status = sem_wait(sem);
        if(status == -1){
            perror("sem_wait: sem_wait failed");
            exit(1);
        }
        cout << "Hijo: soy el proceso " << getpid() << " y tengo el semaforo" << endl;
        sleep(2);
        status = sem_post(sem);
        if(status == -1){
            perror("sem_post: sem_post failed");
            exit(1);
        }
        status = sem_close(sem);
        if(status == -1){
            perror("sem_close: sem_close failed");
            exit(1);
        }
    } else {
        for(int i = 0; i < 10; i++){
            int status;
            pid = wait(&status);
            cout << "Padre: termino el proceso " << pid << endl;
        }
        cout << "Padre: terminaron todos" << endl;
    }

    return 0;
}
