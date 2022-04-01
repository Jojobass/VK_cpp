#include <ctime>
#include <cstdlib>

extern "C" {
#include <malloc.h>
#include "test_successive.h"
#include "test_parallel.h"
}

#define SIZE 100000000

int main() {
    int* big_arr = (int *)malloc(sizeof(int) * SIZE);

    double time_successive = 0;
    double time_parallel = 0;
    char *buf = (char *) "(((x>1)||(x<0))&&((x<=-30)==(x==-55)))";

    for (int i = 0; i < 10; ++i) {
        srand(time(NULL));
        for (int j = 0; j < SIZE; ++j)
            big_arr[j] = rand() % 100 - 60;

        clock_t begin_successive = clock();
        printf("Последовательная: %d, ", successive_func(buf, big_arr, SIZE));
        clock_t end_successive = clock();

        time_successive += (double) (end_successive - begin_successive) / CLOCKS_PER_SEC;
        printf("%f c.\n", time_successive);

        clock_t begin_parallel = clock();
        printf("Параллельная: %d, ", parallel_func(buf, big_arr, SIZE));
        clock_t end_parallel = clock();

        time_parallel += (double) (end_parallel - begin_parallel) / CLOCKS_PER_SEC;
        printf("%f c.\n", time_parallel);
    }
    printf("Время выполнения статичной библиотекой: %f\n", time_successive / 10);
    printf("Время выполнения динамической библиотекой: %f\n", time_parallel / 10);


    return 0;
}