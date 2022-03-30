#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>
#include "bool_expr_lib.h"
#include "../../polish_notation_lib/int_comparison/include/polish_int_comp_lib.h"

#define DEFAULT_INITIAL_LENGTH 1


static char pop_buffer_string(char **buffer, size_t length) {
    char popped = *buffer[length - 1];
    char *new_buffer = NULL;
    if (!(new_buffer = (char *) malloc(sizeof(char) * length))) {
        perror("Allocation error!");
        return 0;
    }
    // Copying data from the old buffer to the new one
    memcpy(new_buffer, *buffer, (length - 1) * sizeof(char));
    new_buffer[length - 1] = '\0';
    // Freeing the old memory
    free(*buffer);


    // Setting new pointer
    *buffer = new_buffer;

    return popped;
}

typedef bool (*functype)(void *a, void *);

static int resize_buffer_funcs(functype **buffer, size_t old_length, size_t new_length) {
    if (new_length <= old_length) {
        perror("Buffer length error!");
        return 2;
    }

    // Allocating new buffer with check
    functype *new_buffer = NULL;
    if (!(new_buffer = malloc(sizeof(functype) * new_length))) {
        perror("Allocation error!");
        return 1;
    }

    if (old_length > 0) {
        // Copying data from the old buffer to the new one
        memcpy(new_buffer, *buffer, old_length * sizeof(functype));
        // Freeing the old memory
        free(*buffer);
    }

    // Setting new pointer
    *buffer = new_buffer;

    return 0;
}

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
    }
    return num;
}
