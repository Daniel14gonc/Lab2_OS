#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    fork();
    fork();
    fork();
    fork();
    printf("Proceso\n");
    return 0;
}
