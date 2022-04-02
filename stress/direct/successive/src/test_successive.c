#include "test_successive.h"
#include "bool_expr_lib.h"

int successive_func(char* expr, int* arr, size_t arr_size){
    return get_num_passed(expr, arr, arr_size);
}
