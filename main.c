#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include "bool_expr_lib.h"
//#include "bool_expr_successive_lib.h"

int main() {
    int *big_arr = malloc(100000000 * sizeof(int));
    srand(time(NULL));
    for (size_t i = 0; i < 100000000; ++i) {
        big_arr[i] = rand()%80 + 70;
    }
    int probe[1] = {1};
    if(get_num_passed("((100 < x) && (x <= 120))", probe, 1) != -2){
        printf("%d", get_num_passed("((100 < x) && (x <= 120))", big_arr, 100000000));
    } else {
        printf("%d", get_num_passed_("((100 < x) && (x <= 120))", big_arr, 100000000));
    }

    free(big_arr);
    return 0;
}
