#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdio.h>
#include <stdlib.h>

void shm_deleter(key_t key)
{
    int shmid;

    shmid = shmget(key, 0, 0);
    if (shmid < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    key_t key1;
    key_t key2;

    char *path1 = "sys1.c";
    char *path2 = "sys2.c";

    int proj1 = 1;
    int proj2 = 0;

    key1 = ftok(path1, proj1);
    if (key1 < 0) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    key2 = ftok(path2, proj2);
    if (key2 < 0) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    shm_deleter(key1);
    shm_deleter(key2);

    return 0;
}