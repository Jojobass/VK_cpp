extern "C" {
#include <malloc.h>
#include "bool_expr_lib.h"
}

int main() {
    int *arr = (int *) malloc(sizeof(int) * 1);
    char *expr = (char *) "x>1";
    arr[0] = 1;
    printf("Got %d, expected %d.", get_num_passed_(expr, arr, 1), 0);
    arr[0] = 2;
    printf("Got %d, expected %d.", get_num_passed_(expr, arr, 1), 1);
    free(arr);
}