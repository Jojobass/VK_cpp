extern "C" {
#include <malloc.h>
#include "bool_expr_lib.h"
}

int main(){
    int *arr = (int *) malloc(sizeof(int) * 2);
    char *expr = (char *) "x>1";
    arr[0] = 1;
    arr[1] = 0;
    printf("Got %d, expected %d.", get_num_passed_(expr, arr, 2), 0);
    arr[0] = 1;
    arr[1] = 2;
    printf("Got %d, expected %d.", get_num_passed_(expr, arr, 2), 1);
    arr[0] = 2;
    arr[1] = 3;
    printf("Got %d, expected %d.", get_num_passed_(expr, arr, 2), 2);
    free(arr);
}
