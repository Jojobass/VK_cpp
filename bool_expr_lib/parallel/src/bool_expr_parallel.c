#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bool_expr_lib.h"

#include "polish_int_comp_lib.h"

static int get_part(char **polish, int polish_size, int *arr, size_t part_size) {
    int num = 0;
    for (size_t i = 0; i < part_size; ++i) {
        if (pass_bool_expr(polish, polish_size, arr[i]) == 1) { ++num; }
    }
    return num;
}

int get_num_passed(char *expr, int *arr, size_t arr_size) { return -2; }

int get_num_passed_(char *expr, int *arr, size_t arr_size) {
    if (!check_valid(&expr)) {
        perror("NOT VALID\n");
        return -1;
    }

    char **polish;
    int polish_size = detect_functions(expr, &polish);

//    максимальное количество одновременных процессов
    int num_of_processes = sysconf(_SC_NPROCESSORS_ONLN);
//    массив с PID всех процессов
    pid_t pids[num_of_processes];
//    массив, который могут читать и изменять все процессы
    int *num = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                    -1, 0);
    if (num == MAP_FAILED) { return -1; }

//    создание процессов и одновременная проверка ~равных частей массива
    for (int i = 0; i < num_of_processes; ++i) {
        pids[i] = fork();
        if (pids[i] == 0) {
            if (i != num_of_processes - 1) {
                *num += get_part(polish, polish_size, &(arr[(arr_size / num_of_processes) * i]),
                                 arr_size / num_of_processes);
            } else {
                *num += get_part(polish, polish_size, &(arr[(arr_size / num_of_processes) * i]),
                                 arr_size / num_of_processes + arr_size % num_of_processes);
            }

            exit(EXIT_SUCCESS);
        }
    }
//    проверка на зомби
    for (int i = 0; i < num_of_processes; ++i) {
        if (waitpid(pids[i], NULL, 0) != pids[i]) {
            return -1;
        }
    }

    int res = *num;
    if (munmap(num, sizeof(int)) != 0) {
        perror("Unmapping error!\n");
        return -1;
    }
    for(int i=0; i<polish_size; ++i){
        free(polish[i]);
    }
    free(polish);
    return res;
}
