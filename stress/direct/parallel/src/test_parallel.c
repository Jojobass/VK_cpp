#include "test_parallel.h"
#include "bool_expr_lib.h"

int parallel_func(char* expr, int* arr, size_t arr_size){
    return get_num_passed_(expr, arr, arr_size);
}
