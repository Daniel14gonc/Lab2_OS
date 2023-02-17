#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int size = 1024;
    char* letra = (char*) argv[2];
    long n = strtol(argv[1], NULL, 10);

    printf("Soy %s\n", letra);

    char* key = "shared";
    int shm_d;

    char* myfifo = "fifo_x";
    mkfifo(myfifo, 0666);
    char arr1[80];

    shm_d = shm_open(key,O_CREAT | O_RDWR,S_IRUSR|S_IWUSR);
    int nbytes;
    int fd[2];
    pipe(fd);
    int p[2];
    pipe(p);
    ftruncate(shm_d, size);

    char* ptr = (char*) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_d, 0);
    int pid = fork();

    if (pid == 0) {
        close(fd[1]);
        char buff[2];
        nbytes = 1;
        while(nbytes > 0) {
            nbytes = read(fd[0], buff, 2);
            if (strcmp(buff, "t") == 0) {
                break;
            }
            else {
                int i = 0;
                while (ptr[i] != '\0') i++;
                ptr[i] = letra[0];
            }
            
        }
    }
    else {
        close(fd[0]);
        for (int i = 0; i < size; i++) {
            if(i % n == 0) {
                write(fd[1], letra, strlen(letra) + 1);
            }
        }   
        write(fd[1], "t", strlen("t") + 1);
        wait(NULL);
        printf("%s: Contenido en memoria: %s\n", letra, (char*) ptr);
    }
    shm_unlink(key);
    return 0;
}