#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t id1;
    pid_t id2;
    pid_t id3;
    clock_t start, end;

    start = clock();
    id1 = fork();
    if (id1 == 0) {
        int id2 = fork();
        if (id2 == 0) {
            id3 = fork();
            if (id3 == 0) {
                for (int i = 0; i < 1000000; i++)
                    printf("%d\n", i);
            } 
            else {
                for (int i = 0; i < 1000000; i++)
                    printf("%d\n", i);;
                wait(NULL);
            }
        }
        else {
            for (int i = 0; i < 1000000; i++)
                printf("%d\n", i);;
            wait(NULL);
        }
    }
    else {
        wait(NULL);
        end = clock();
        double difference = (double) (end - start);
        printf("Tiempo: %f\n", difference);
    }

    return 0;
}