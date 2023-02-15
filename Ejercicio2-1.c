#include <stdio.h>
#include <time.h>

int main() {
    clock_t start;
    clock_t end;
    
    start = clock();
    for (int i = 0; i < 1000000; i++)
        printf("%d\n", i);;
    for (int i = 0; i < 1000000; i++)
        printf("%d\n", i);;
    for (int i = 0; i < 1000000; i++)
        printf("%d\n", i);;
    end = clock();

    double difference = (double) (end - start);

    printf("Time: %f\n", difference);

    return 0;
}