#include "bool_expr_lib.h"
#include "polish_int_comp_lib.h"

int get_num_passed(char *expr, int *arr, size_t arr_size) {
    int num = 0;
    if (!check_valid(&expr)) {
        perror("NOT VALID\n");
        return -1;
    }
    char **polish;
    int polish_size = detect_functions(expr, &polish);
    free(expr);
    for (size_t i = 0; i < arr_size; ++i) {
        if (pass_bool_expr(polish, polish_size, arr[i]) == 1) { ++num; }
    }
    for (int i = 0; i < polish_size; ++i) {
        free(polish[i]);
    }
    free(polish);
    return num;
}
