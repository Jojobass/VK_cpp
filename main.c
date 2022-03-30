#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include "bool_expr_lib.h"

int main() {
    int *big_arr = malloc(100000000 * sizeof(int));
    srand(time(NULL));
    for (size_t i = 0; i < 100000000; ++i) {
        big_arr[i] = rand()%80 + 70;
    }
    printf("%d", get_num_passed("((100 < x) && (x <= 120))", big_arr, 100000000));
    return 0;
}
