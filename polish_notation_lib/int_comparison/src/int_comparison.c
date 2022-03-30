#include "polish_int_comp_lib.h"

static int resize_buffer_string_array(char ***buffer, size_t old_length, size_t new_length) {
    // Allocating new buffer with check
    char **new_buffer = NULL;
    if (!(new_buffer = (char **) malloc(sizeof(char *) * new_length))) {
        perror("Allocation error!");
        return 1;
    }

    if (old_length > 0) {
        if (new_length >= old_length) {
            // Copying data from the old buffer to the new one
            memcpy(new_buffer, *buffer, old_length * sizeof(char *));
            // Freeing the old memory
            free(*buffer);
        } else {
            // Copying data from the old buffer to the new one
            memcpy(new_buffer, *buffer, new_length * sizeof(char *));
            // Freeing the old memory
            free(*buffer);
        }
    }

    // Setting new pointer
    *buffer = new_buffer;

    return 0;
}

static int resize_buffer_string(char **buffer, size_t old_length, size_t new_length) {
    if (new_length <= old_length) {
        perror("Buffer length error!");
        return 2;
    }

    // Allocating new buffer with check
    char *new_buffer = NULL;
    if (!(new_buffer = (char *) malloc(sizeof(char) * new_length))) {
        perror("Allocation error!");
        return 1;
    }

    if (old_length > 0) {
        // Copying data from the old buffer to the new one
        memcpy(new_buffer, *buffer, old_length * sizeof(char));
        // Freeing the old memory
        free(*buffer);
    }

    // Setting new pointer
    *buffer = new_buffer;

    return 0;
}

static int remove_white_spaces(char **str) {
    int i = 0, j = 0;
    char *res = malloc(strlen(*str) + 1);
    while ((*str)[i]) {
        if ((*str)[i] != ' ') {
            res[j] = (*str)[i];
            ++j;
        }

        i++;
    }
    res[j] = '\0';
    *str = res;
    return 0;
}

bool check_valid(char **expr_) {
    remove_white_spaces(expr_);
    int brackets = 0;
    int and_or_brackets = 0;
    int and_or_num = 0;
    for (int i = 0; i < strlen(*expr_); ++i) {
        if ((*expr_)[i] == '(') {
            ++brackets;
            if (i > 0) {
                if (!strchr("=&|(", (*expr_)[i - 1])) { return false; }
                if (i > 2) {
                    if ((*expr_)[i - 1] == '=' && (*expr_)[i - 2] != '=') { return false; }
                }
            }
            if (i >= strlen(*expr_) - 5) { return false; }
            if ((*expr_)[i + 1] == '(') { ++and_or_brackets; }
            continue;
        }
        if ((*expr_)[i] == ')') {
            --brackets;
            if (brackets < 0) { return false; }
            if ((*expr_)[i - 1] == '(') { return false; }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x' &&
                (*expr_)[i - 1] != ')') { return false; }
            if ((*expr_)[i - 1] == ')') { ++and_or_brackets; }
            continue;
        }
        if ((*expr_)[i] == '&') {
            if (i > strlen(*expr_) - 7 || i < 5) { return false; }
            if (brackets == 0) { return false; }
            if ((*expr_)[i + 1] != '&') { return false; }
            if ((*expr_)[i - 1] != ')' || (*expr_)[i + 2] != '(') { return false; }
            ++i;
            ++and_or_num;
            continue;
        }
        if ((*expr_)[i] == '|') {
            if (i > strlen(*expr_) - 7 || i < 5) { return false; }
            if (brackets == 0) { return false; }
            if ((*expr_)[i + 1] != '|') { return false; }
            if ((*expr_)[i - 1] != ')' || (*expr_)[i + 2] != '(') { return false; }
            ++i;
            ++and_or_num;
            continue;
        }
        if ((*expr_)[i] == '=') {
            if (i >= strlen(*expr_) - 2 || i < 1) { return false; }
            if ((*expr_)[i + 1] != '=') { return false; }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x' &&
                (*expr_)[i - 1] != ')') { return false; }
            ++i;
            continue;
        }
        if ((*expr_)[i] == '>' || (*expr_)[i] == '<') {
            if (i > strlen(*expr_) - 2 || i < 1) { return false; }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x') { return false; }
            if ((*expr_)[i + 1] == '=') {
                if (i + 1 == strlen(*expr_) - 1) { return false; }
                ++i;
            }
            continue;
        }
        if (isdigit((*expr_)[i])) {
            if (i != 0) {
                if (strchr("><(=", (*expr_)[i - 1]) == NULL) { return false; }
            }
            while (isdigit((*expr_)[i])) {
                ++i;
            }
            --i;
            continue;
        }
        if ((*expr_)[i] == 'x') {
            if (i != 0) {
                if (strchr("><(=", (*expr_)[i - 1]) == NULL) { return false; }
            }
            continue;
        }
    }
    if (brackets != 0) { return false; }
    if (and_or_brackets != and_or_num * 2) { return false; }
    return true;
}

int detect_functions(char *expr, char ***polish) {
//  counting num of operators and brackets
    int num_of_operators = 0;
    int num_of_brackets = 0;
    for (int i = 0; i < strlen(expr); ++i) {
        if (expr[i] == '(' || expr[i] == ')') {
            ++num_of_brackets;
            continue;
        }
        if (expr[i] == '&' && expr[i + 1] == '&') {
            ++num_of_operators;
            ++i;
            continue;
        }
        if (expr[i] == '|' && expr[i + 1] == '|') {
            ++num_of_operators;
            ++i;
            continue;
        }
        if (expr[i] == '=' && expr[i + 1] == '=') {
            ++num_of_operators;
            ++i;
            continue;
        }
        if (expr[i] == '<' || expr[i] == '>') {
            if (expr[i + 1] == '=') {
                ++num_of_operators;
                ++i;
                continue;
            } else {
                ++num_of_operators;
                continue;
            }
        }
    }

    char **functions;
//  filling array in polish notation
    int num_of_elems = 0;
    int oper_arr_size = 0;
    for (int i = 0; i < strlen(expr) + 1; ++i) {
        int string_size = 0;
        if (isdigit(expr[i])) {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 1);
            ++string_size;
            while (isdigit(expr[i])) {
                resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 1);
                (*polish)[num_of_elems][string_size - 1] = expr[i];
                ++string_size;
                ++i;
            }
            (*polish)[num_of_elems][string_size - 1] = '\0';
            ++num_of_elems;
            --i;
            continue;
        }
        if (expr[i] == 'x') {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 2);
            (*polish)[num_of_elems][0] = 'x';
            (*polish)[num_of_elems][1] = '\0';
            ++num_of_elems;
            continue;
        }
        if (strchr("&|=", expr[i]) != NULL && expr[i] != '\0') {
            resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size + 1);
            resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 3);
            functions[oper_arr_size][0] = expr[i];
            functions[oper_arr_size][1] = expr[i + 1];
            functions[oper_arr_size][2] = '\0';
            ++i;
            ++oper_arr_size;
            continue;
        }
        if (strchr("<>", expr[i]) != NULL && expr[i] != '\0') {
            resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size + 1);
            if (expr[i + 1] == '=') {
                resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 3);
                functions[oper_arr_size][0] = expr[i];
                functions[oper_arr_size][1] = expr[i + 1];
                functions[oper_arr_size][2] = '\0';
                ++i;
            } else {
                resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 2);
                functions[oper_arr_size][0] = expr[i];
                functions[oper_arr_size][1] = '\0';
            }
            ++oper_arr_size;
            continue;
        }
        if (expr[i] == ')') {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            resize_buffer_string(&((*polish)[num_of_elems]), string_size,
                                 strlen(functions[oper_arr_size - 1]) + 1);
            strcpy((*polish)[num_of_elems], functions[oper_arr_size - 1]);
            free(functions[oper_arr_size - 1]);
            resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size - 1);
            --oper_arr_size;
            ++num_of_elems;
            continue;
        }
        if (expr[i] == '\0') {
            while (functions[0]) {
                resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
                resize_buffer_string(&((*polish)[num_of_elems]), string_size,
                                     strlen(functions[oper_arr_size - 1]) + 1);
                strcpy((*polish)[num_of_elems], functions[oper_arr_size - 1]);
                free(functions[oper_arr_size - 1]);
                resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size - 1);
                --oper_arr_size;
                ++num_of_elems;
            }
        }
    }
    return num_of_elems;
}

bool and(bool a, bool b) { return a && b; }

bool or(bool a, bool b) { return a || b; }

bool equal(int a, int b) { return a == b; }

bool lt(int a, int b) { return a < b; }

bool mt(int a, int b) { return a > b; }

bool let(int a, int b) { return a <= b; }

bool met(int a, int b) { return a >= b; }

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
    perror("Operator identifying error!\n");
    return -1;
}

int pass_bool_expr(char** polish, int polish_size, int x) {
    int *results;
    int results_size = 0;
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
