#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int destructionZDC(int id);

int main() {
    key_t cle;
    int shmid, i, j;
    char *pZDC = NULL, *pZone = NULL;

    cle = ftok("/usr/include/err.h", 0);
    shmid = shmget(cle, 50, IPC_EXCL);
    if(shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }

    pZone = malloc(sizeof(char) * 50);
    if(pZone == NULL) {
        perror("pZone malloc");
        return EXIT_FAILURE;
    }

    for(i = 0; i < 30; i++) {
        pZDC = shmat(shmid, NULL, 0);
        if(pZDC == (char*)-1) {
            destructionZDC(shmid);
            perror("shmat");
            return EXIT_FAILURE;
        }

        for(j = 0; j < 50; j++) {
            pZone[j] = pZDC[j];
        }

        printf("%s\n", pZone);

        if(shmdt(pZDC) == -1) {
            destructionZDC(shmid);
            perror("shmdt");
            return EXIT_FAILURE;
        }

        sleep(2);
    }

    destructionZDC(shmid);

    printf("Lecture fini !");

    return 0;
}

int destructionZDC(int id) {
    return shmctl(id, IPC_RMID, NULL);
}
