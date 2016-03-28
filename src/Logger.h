#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#define FILE_PATH "../log.txt"

union semun {
        int val;                /* value for SETVAL */
        struct semid_ds *buf;   /* buffer for IPC_STAT & IPC_SET */
        ushort *array;          /* array for GETALL & SETALL */
        struct seminfo *__buf;  /* buffer for IPC_INFO */
};

class Logger
{
    public:
        Logger();
        int push(std::string message);
        int write(std::string message);
        virtual ~Logger();
    protected:
    private:
        std::vector<std::string> message_queue;
        int semid;
        int binary_semaphore_allocation (key_t key, int sem_flag);
        int binary_semaphore_deallocate (int semid);
};

#endif // LOGGER_H
