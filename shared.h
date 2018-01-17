#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define SHM_KEY 45



int semaphore_create(int key)
{
    return semget(key, 1, 0666 | IPC_CREAT);
}

int semaphore_open(int key)
{
    return semget(key, 1, 0666);
}

int semaphore_remove(int id)
{
    return semctl(id, 0, IPC_RMID);
}

int semaphore_init(int id, int val)
{
    return semctl(id, 0, SETVAL, val);
}

int up(int id)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;
    
    return semop(id, &op, 1);
}

int down(int id)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    return semop(id, &op, 1);
}

#endif
