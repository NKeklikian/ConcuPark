#include "Logger.h"

Logger::Logger()
{
    //ctor
}

int Logger::write(std::string message) {
    return -1;
}

int Logger::push(std::string message) {
    this->message_queue.push_back(message);
    return 0;
}

int Logger::binary_semaphore_allocation(key_t key, int sem_flag) {
    return semget(key,1,sem_flag);
}

int Logger::binary_semaphore_deallocate(int semid) {
    union semun not_used;
    return semctl(semid,1,IPC_RMID,not_used);
}

Logger::~Logger()
{
    binary_semaphore_deallocate(semid);
}
