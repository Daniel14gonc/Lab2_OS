#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int x,y;
    x = fork();
    if (x!=0) {
        y = fork();
        if (y==0) {
            fork();
            printf("hola\n");
        }
    }
    else {
        fork();
    }

    fork();
    printf("hola\n");
    return 0;
}