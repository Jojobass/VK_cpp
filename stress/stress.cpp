#include <ctime>
#include <cstdlib>

extern "C" {
#include <malloc.h>
#include "test_parallel.h"
#include "test_successive.h"
}

#define SIZE 100000000
#define REPEAT 3

int main() {
    int* big_arr = (int *)malloc(sizeof(int) * SIZE);

    double time_successive = 0;
    double time_parallel = 0;
    char *buf = (char *) "(((x>1)||(x<0))&&((x<=-30)==(x==-55)))";

    for (int i = 0; i < REPEAT; ++i) {
        srand(time(NULL));
        for (int j = 0; j < SIZE; ++j)
            big_arr[j] = rand() % 100 - 60;

        timespec begin_successive, end_successive;
        clock_gettime(CLOCK_REALTIME, &begin_successive);
        printf("Последовательная: %d, ", successive_func(buf, big_arr, SIZE));
        clock_gettime(CLOCK_REALTIME, &end_successive);

//        time_successive += (double) (end_successive - begin_successive) / CLOCKS_PER_SEC;
        time_successive += (double)(end_successive.tv_sec - begin_successive.tv_sec)+(double)(end_successive.tv_nsec - begin_successive.tv_nsec)/1000000000;
        printf("%f c.\n", time_successive);

        timespec begin_parallel, end_parallel;
        clock_gettime(CLOCK_REALTIME, &begin_parallel);
        printf("Параллельная: %d, ", parallel_func(buf, big_arr, SIZE));
        clock_gettime(CLOCK_REALTIME, &end_parallel);

//        time_parallel += (double) (end_parallel - begin_parallel) / CLOCKS_PER_SEC;
        time_parallel += (double)(end_parallel.tv_sec - begin_parallel.tv_sec)+(double)(end_parallel.tv_nsec - begin_parallel.tv_nsec)/1000000000;

        printf("%f c.\n", time_parallel);
    }
    printf("Время выполнения статичной библиотекой: %f\n", time_successive / REPEAT);
    printf("Время выполнения динамической библиотекой: %f\n", time_parallel / REPEAT);

    free(big_arr);
    return 0;
}