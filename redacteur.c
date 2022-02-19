#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int destructionZDC(int id);

int main() {
    key_t cle;
    int shmid, i, j, etoile = 1;
    char *pZDC = NULL;

    cle = ftok("/usr/include/err.h", 0);
    shmid = shmget(cle, 50, IPC_CREAT | IPC_EXCL | 0600);

    if(shmid == -1) {
        shmid = shmget(cle, 50, IPC_EXCL);
        if(shmid == -1) {
            perror("shmget");
            return EXIT_FAILURE;
        }
    }

    for(i = 0; i < 20; i++) {
        pZDC = shmat(shmid, NULL, 0);
        if(pZDC == (char*)-1) {
            destructionZDC(shmid);
            perror("shmat");
            return EXIT_FAILURE;
        }

        if(etoile) {
            for(j = 0; j < 50; j++) {
                pZDC[j] = '*';
                if(!(j % 10)) {
                    sleep(0.7);
                }
            }
            etoile = 0;
            sleep(1);
        } else {
            for(j = 0; j < 50; j++) {
                pZDC[j] = '#';
                if(!(j % 8)) {
                    sleep(1);
                }
            }
            etoile = 1;
        }

        if(shmdt(pZDC) == -1) {
            destructionZDC(shmid);
            perror("shmdt");
            return EXIT_FAILURE;
        }
    }

    destructionZDC(shmid);

    printf("Redaction terminer !");

    return 0;
}

int destructionZDC(int id) {
    return shmctl(id, IPC_RMID, NULL);
}
