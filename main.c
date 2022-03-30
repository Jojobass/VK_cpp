#include <stdio.h>
#include "bool_expr_lib.h"

int main() {
    printf("%d", pass_bool_expr("(( 123 <x) && ((x >=12) || (x == 120)))", 124));
    return 0;
}
