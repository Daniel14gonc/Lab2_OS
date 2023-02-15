#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#define MEMORY_SPACE 4096 * sizeof(char)

int main(int argc, char** argv) {
    if (argc > 0) {
        char* name = (char*) argv[2];
        long temp = strtol(argv[1], NULL, 10);
        int n = (int) temp;

        printf("I am %s\n", name);

        char* key = "archive";
        int fileDescriptor;

        char* myfifo = "/tmp/myfifo";
        mkfifo(myfifo, 0666);
        char arr1[80];

        fileDescriptor = shm_open(key,O_CREAT | O_EXCL,S_IRUSR|S_IWUSR);
        if (fileDescriptor != -1) {
            fileDescriptor = shm_open(key,O_RDWR,S_IRUSR|S_IWUSR);
            printf("%s: Created new shared mem obj %d\n", name, fileDescriptor);
            char arr2[80];
            sprintf(arr2, "%d", fileDescriptor);
            int pipeFd = open(myfifo, O_WRONLY);
            write(pipeFd, arr2, strlen(arr2));
            close(pipeFd);
        }
        else {
            fileDescriptor = shm_open(key,O_RDWR,S_IRUSR|S_IWUSR);
            printf("%s: Shared mem obj already created\n", name);
            int pipeFd = open(myfifo, O_RDONLY);
            read(pipeFd, arr1, sizeof(arr1));
            int fileDescriptorReceived = atoi(arr1);
            close(pipeFd);
            printf("%s: File descriptor sent to me-> %d\n", name, fileDescriptorReceived);
        }
        ftruncate(fileDescriptor, MEMORY_SPACE);
        int pipeFd1, pipeFd2;
        int nbytes;


        int fd[2];
        pipe(fd);

        int p[2];
        if (pipe(p) < 0)
            exit(1);
        

        char* ptr = (char *) mmap(NULL, MEMORY_SPACE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
        int pid = fork();

        if (pid == 0) {

            close(fd[1]);
            char buff[2];
            bool finished = false;
            nbytes = 1;
            while(!finished && nbytes > 0) {
                nbytes = read(fd[0], buff, 2);
                if (strcmp(buff, "f") == 0) {
                    finished = true;
                }
                else {
                    int i = 0;
                    while (*ptr != '\0') ptr++;
                    sprintf(ptr, "%s", buff);
                }
                
            }
        }
        else {
            close(fd[0]);
            for (int i = 0; i < 10; i++) {
                if(i % n == 0) {
                    write(fd[1], name, strlen(name) + 1);
                }
            }   
            write(fd[1], "f", strlen("f") + 1);
            wait(NULL);
            printf("\n%s: Content in shared memory: %s\n", name, ptr);

            usleep(5000);
        }
        shm_unlink(key);
    }
    else {
        return -1;
    }
    return 0;
}