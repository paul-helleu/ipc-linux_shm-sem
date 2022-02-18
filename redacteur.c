#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

int creationZDC(char *nomFichier, int taille);
char *attacheZDC(int id);
int detacheZDC(char *pZDC);
int destructionZDC(int id);

int main() {
    int shmId;
    char *pZDC;
    int etoile = 1;
    int i, j;

    if((shmId = creationZDC("/usr/include/err.h", 50)) == -1) {
        perror("creationZDC");
        return EXIT_FAILURE;
    }

    for(i = 0; i < 30; i++) {
        if((pZDC = attacheZDC(shmId)) == (char*)-1) {
            destructionZDC(shmId);
            perror("attacheZDC");
            return EXIT_FAILURE;
        }

        if(etoile) {
            for(j = 0; j < 50; j++) {
                pZDC[j] = '*';
                if(j % 10) {
                    sleep(1);
                }
            }
            etoile = 0;
        } else {
            for(j = 0; j < 50; j++) {
                pZDC[j] = '#';
                if(j % 6) {
                    sleep(0.7);
                }
            }
            etoile = 1;
        }

        if(etoile) {
            sleep(0.5);
        } else
            sleep(1);

        if(detacheZDC(pZDC) == -1) {
            destructionZDC(shmId);
            perror("detacheZDC");
            return EXIT_FAILURE;
        }
    }

    destructionZDC(shmId);

    printf("Redaction terminer !");

    return 0;
}

int creationZDC(char *nomFichier, int taille) {
    key_t cle = ftok(nomFichier, 0);

    int shmid = shmget(cle, taille, IPC_CREAT | IPC_EXCL | 0600);
    if(shmid == -1) {
        shmid = shmget(cle, taille, IPC_EXCL);
        if(shmid == -1) {
            return -1;
        }
    }
    return shmid;
}

char *attacheZDC(int id) {
    return shmat(id, NULL, 0);
}

int detacheZDC(char *pZDC) {
    return shmdt(pZDC);
}

int destructionZDC(int id) {
    return shmctl(id, IPC_RMID, NULL);
}
