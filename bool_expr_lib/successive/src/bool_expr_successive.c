#include "bool_expr_lib.h"
//#include "../../polish_notation_lib/int_comparison/include/polish_int_comp_lib.h"
#include "polish_int_comp_lib.h"

int get_num_passed(char *expr, int *arr, size_t arr_size) {
    int num = 0;
    if (!check_valid(&expr)) {
        perror("NOT VALID\n");
        return -1;
    }
    int buf = 0;
    char **polish;
    int polish_size = detect_functions(expr, &polish);
    for (size_t i = 0; i < arr_size; ++i) {
        if (pass_bool_expr(polish, polish_size, arr[i]) == 1) { ++num; }
        if (i == arr_size / 8) {
            printf("1/8\n");
        }
        if (i == arr_size / 4) { printf("1/4\n"); }
        if (i == arr_size / 2) { printf("1/2\n"); }
        if (i == arr_size * 3 / 4) { printf("3/4\n"); }
    }
    for(int i=0; i<polish_size; ++i){
        free(polish[i]);
    }
    free(polish);
    return num;
}
