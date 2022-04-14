#include "test_parallel.h"
#include "bool_expr_lib.h"
#include <sys/mman.h>
#include <unistd.h>

int parallel_func(char* expr, int* arr, size_t arr_size){
    return get_num_passed_(expr, arr, arr_size, (int) sysconf(_SC_NPROCESSORS_ONLN));
}
