#include <iostream>
#include "Parque.h"
#include <unistd.h>
#include <wait.h>

int main(){
    using namespace std;
    cout << "Hello, world!" << endl;
    pid_t pid = fork();
    if (pid == 0){
        cout << "Proceso hijo!" << endl;
        cout << getpid() << endl;
        Parque* parque = new Parque();
        delete parque;
    } else {
        wait(NULL);
        cout << "Proceso padre!" << endl;
        cout << getpid() << endl;
    }
    return 0;
}
