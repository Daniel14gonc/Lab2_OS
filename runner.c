#include <unistd.h>

int main() {
    char* path = "./ipc";
    char* firstSymbol = "a";
    char* secondSymbol = "b";
    char* n1 = "3";
    char* n2 = "4";
    if (fork() == 0) {
        execl(path, path, n1, firstSymbol,(char*) NULL);
    }
    else {
        usleep(500);
        execl(path, path, n2, secondSymbol,(char*) NULL);
    }
    return 0;
}