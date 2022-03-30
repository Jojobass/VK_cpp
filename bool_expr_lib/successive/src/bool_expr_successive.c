#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>
#include "bool_expr_lib.h"
#include "../../polish_notation_lib/int_comparison/include/polish_int_comp_lib.h"

#define DEFAULT_INITIAL_LENGTH 1

bool and(bool a, bool b) { return a && b; }

bool or(bool a, bool b) { return a || b; }

bool equal(int a, int b) { return a == b; }

bool lt(int a, int b) { return a < b; }

bool mt(int a, int b) { return a > b; }

bool let(int a, int b) { return a <= b; }

bool met(int a, int b) { return a >= b; }

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

static int resize_buffer_ints(int **buffer, size_t old_length, size_t new_length) {
    // Allocating new buffer with check
    int *new_buffer = NULL;
    if (!(new_buffer = (int *) malloc(sizeof(int) * new_length))) {
        perror("Allocation error!");
        return 1;
    }

    if (old_length > 0) {
        if (new_length >= old_length) {
            // Copying data from the old buffer to the new one
            memcpy(new_buffer, *buffer, old_length * sizeof(int));
            // Freeing the old memory
            free(*buffer);
        } else {
            // Copying data from the old buffer to the new one
            memcpy(new_buffer, *buffer, new_length * sizeof(int));
            // Freeing the old memory
            free(*buffer);
        }
    }

    // Setting new pointer
    *buffer = new_buffer;

    return 0;
}


int use_operator(char *oper, int a, int b) {
    if (!strcmp(oper, "<")) { return lt(a, b); }
    if (!strcmp(oper, "<=")) { return let(a, b); }
    if (!strcmp(oper, ">")) { return mt(a, b); }
    if (!strcmp(oper, ">=")) { return met(a, b); }
    if (!strcmp(oper, "&&")) { return and(a, b); }
    if (!strcmp(oper, "||")) { return or(a, b); }
    if (!strcmp(oper, "==")) { return equal(a, b); }
}

int pass_bool_expr(char *expr, int x) {
    if (!check_valid(&expr)) {
        perror("NOT VALID\n");
        return -1;
    }
    char **polish;
    int *results;
    int results_size = 0;
    int polish_size = detect_functions(expr, &polish);
    for (int i = 0; i < polish_size; ++i) {
        if (isdigit(polish[i][0])) {
            char *p;
            resize_buffer_ints(&results, results_size, results_size + 1);
            results[results_size] = (int) strtol(polish[i], &p, 10);
            ++results_size;
            continue;
        }
        if (!strcmp(polish[i], "x")) {
            resize_buffer_ints(&results, results_size, results_size + 1);
            results[results_size] = x;
            ++results_size;
            continue;
        }
        int buf = use_operator(polish[i], results[results_size - 2], results[results_size - 1]);
        resize_buffer_ints(&results, results_size, results_size - 1);
        --results_size;
        results[results_size - 1] = buf;
    }
    if (results_size != 1){
        perror("Decoding error!\n");
        return -1;
    }
    return results[0];
}
