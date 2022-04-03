#include "polish_int_comp_lib.h"

static int resize_buffer_string_array(char ***buffer, size_t old_length, size_t new_length) {
    // Allocating new buffer with check
    char **new_buffer = NULL;
    if (new_length > 0) {
        if (!(new_buffer = (char **) malloc(sizeof(char *) * new_length))) {
//            if(old_length>0){free(*buffer);}
            perror("Allocation error!");
            return 0;
        }

        if (old_length > 0) {
            if (new_length >= old_length) {
                // Copying data from the old buffer to the new one
                memcpy(new_buffer, *buffer, old_length * sizeof(char *));
                // Freeing the old memory
//            for (int i = 0; i < old_length; ++i) {
//                free((*buffer)[i]);
//            }
                free(*buffer);
            } else {
                // Copying data from the old buffer to the new one
                memcpy(new_buffer, *buffer, new_length * sizeof(char *));
                // Freeing the old memory
//            for (int i = 0; i < old_length-1; ++i) {
//                free((*buffer)[i]);
//            }
                free(*buffer);
            }
        }
    }

    if (old_length > 0 && new_length == 0) { free(*buffer); }
    // Setting new pointer
    *buffer = new_buffer;

    return 1;
}

static int resize_buffer_string(char **buffer, size_t old_length, size_t new_length) {
    if (new_length <= old_length) {
        perror("Buffer length error!");
        return 0;
    }

    // Allocating new buffer with check
    char *new_buffer = NULL;
    if (!(new_buffer = (char *) malloc(sizeof(char) * new_length))) {
        free(new_buffer);
//        if(old_length > 0) {free(*buffer);}
        perror("Allocation error!");
        return 0;
    }

    if (old_length > 0) {
        // Copying data from the old buffer to the new one
        memcpy(new_buffer, *buffer, old_length * sizeof(char));
        // Freeing the old memory
        free(*buffer);
    }

    // Setting new pointer
    *buffer = new_buffer;

    return 1;
}

static int remove_whitespaces(char **str) {
    int i = 0, new_size = 0;
    char *res = NULL;
//    if (!(res = (char *) malloc(strlen(*str) + 1))) {
//        perror("Allocation error!");
//        return 0;
//    }
//    char *res = (char *) malloc(strlen(*str) + 1);

    while (i < strlen(*str)) {
        if ((*str)[i] != ' ') {
            if (new_size == 0) {
                if (!resize_buffer_string(&res, new_size, new_size + 1)) {
//                    if (new_size > 0) { free(res); }
                    perror("Allocation error!\n");
                    return -1;
                }
                ++new_size;
            }
            if (!resize_buffer_string(&res, new_size, new_size + 1)) {
                if (new_size > 0) { free(res); }
//                free(*str);
                perror("Allocation error!\n");
                return 0;
            }
            res[new_size - 1] = (*str)[i];
            ++new_size;
        }

        i++;
    }
    if (new_size < 4) {
        if (new_size > 0) { free(res); }
//        free(*str);
        perror("String error!\n");
        return 0;
    }
    if (new_size > 0) { res[new_size - 1] = '\0'; }
//    free(*str);
    *str = res;
    return 1;
}

bool check_valid(char **expr_) {
    bool is_true = true;
    if (!remove_whitespaces(expr_)) {
//        free(*expr_);
        perror("Trimming error!\n");
        return false;
    }
    int brackets = 0;
    int and_or_brackets = 0;
    int and_or_num = 0;
    if (strlen(*expr_) < 3) {
        free(*expr_);
        return false;
    }
    for (int i = 0; i < strlen(*expr_); ++i) {
        if ((*expr_)[i] == '(') {
            ++brackets;
            if (i > 0) {
                if (!strchr("=&|(", (*expr_)[i - 1])) {
                    is_true = false;
                    break;
                }
                if (i > 2) {
                    if ((*expr_)[i - 1] == '=' && (*expr_)[i - 2] != '=') {
                        is_true = false;
                        break;
                    }
                }
            }
            if (i > strlen(*expr_) - 5) {
                is_true = false;
                break;
            }
            if ((*expr_)[i + 1] == '(') { ++and_or_brackets; }
            continue;
        }
        if ((*expr_)[i] == ')') {
            --brackets;
            if (brackets < 0) {
                is_true = false;
                break;
            }
            if ((*expr_)[i - 1] == '(') {
                is_true = false;
                break;
            }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x' &&
                (*expr_)[i - 1] != ')') {
                is_true = false;
                break;
            }
            if ((*expr_)[i - 1] == ')') { ++and_or_brackets; }
            continue;
        }
        if ((*expr_)[i] == '&') {
            if (i > strlen(*expr_) - 7 || i < 5) {
                is_true = false;
                break;
            }
            if (brackets == 0) {
                is_true = false;
                break;
            }
            if ((*expr_)[i + 1] != '&') {
                is_true = false;
                break;
            }
            if ((*expr_)[i - 1] != ')' || (*expr_)[i + 2] != '(') {
                is_true = false;
                break;
            }
            ++i;
            ++and_or_num;
            continue;
        }
        if ((*expr_)[i] == '|') {
            if (i > strlen(*expr_) - 7 || i < 5) {
                is_true = false;
                break;
            }
            if (brackets == 0) {
                is_true = false;
                break;
            }
            if ((*expr_)[i + 1] != '|') {
                is_true = false;
                break;
            }
            if ((*expr_)[i - 1] != ')' || (*expr_)[i + 2] != '(') {
                is_true = false;
                break;
            }
            ++i;
            ++and_or_num;
            continue;
        }
        if ((*expr_)[i] == '=') {
            if (i >= strlen(*expr_) - 2 || i < 1) {
                is_true = false;
                break;
            }
            if ((*expr_)[i + 1] != '=') {
                is_true = false;
                break;
            }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x' &&
                (*expr_)[i - 1] != ')') {
                is_true = false;
                break;
            }
            if ((*expr_)[i - 1] == ')') {
                if ((*expr_)[i + 2] != '(') {
                    is_true = false;
                    break;
                }
                ++and_or_num;
            }
            ++i;
            continue;
        }
        if ((*expr_)[i] == '>' || (*expr_)[i] == '<') {
            if (i > strlen(*expr_) - 2 || i < 1) {
                is_true = false;
                break;
            }
            if (!isdigit((*expr_)[i - 1]) && (*expr_)[i - 1] != 'x') {
                is_true = false;
                break;
            }
            if ((*expr_)[i + 1] == '=') {
                if (i + 1 == strlen(*expr_) - 1) {
                    is_true = false;
                    break;
                }
                ++i;
            }
            continue;
        }
        if (isdigit((*expr_)[i]) || (*expr_)[i] == '-') {
            if (i != 0) {
                if (strchr("><(=", (*expr_)[i - 1]) == NULL) {
                    is_true = false;
                    break;
                }
            }
            if ((*expr_)[i] == '-') { ++i; }
//            while (isdigit((*expr_)[i])) {
//                ++i;
//                if (i >= strlen(*expr_)) { break; }
//            }
            while (i < strlen(*expr_)) {
                char b = (*expr_)[i];
                if (!isdigit(b)) { break; }
                ++i;
            }

            --i;
            continue;
        }
        if ((*expr_)[i] == 'x') {
            if (i != 0) {
                if (strchr("><(=", (*expr_)[i - 1]) == NULL) {
                    is_true = false;
                    break;
                }
            }
            continue;
        }
        is_true = false;
        break;
    }
    if (brackets != 0) { is_true = false; }
    if (and_or_brackets != and_or_num * 2) { is_true = false; }
    if (!is_true) {
        free(*expr_);
        return false;
    }
//    free(*expr_);
    return true;
}

int detect_functions(char *expr, char ***polish) {
    char **functions = NULL;
//  filling array in polish notation
    int num_of_elems = 0;
    int oper_arr_size = 0;
    for (int i = 0; i < strlen(expr) + 1; ++i) {
        int string_size = 0;
        if (isdigit(expr[i]) || expr[i] == '-') {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            if (!resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 1)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
                if (num_of_elems > 0) { free(*polish); }
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > 0) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
            ++string_size;
            do {
                if (!resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 1)) {
                    for (int j = 0; j < num_of_elems+1; ++j) {
                        free((*polish)[j]);
                    }
                    if (num_of_elems > 0) { free(*polish); }
                    for (int j = 0; j < oper_arr_size; ++j) {
                        free(functions[j]);
                        ++j;
                    }
                    if (oper_arr_size > 0) { free(functions); }
                    perror("Allocation error!\n");
                    return -1;
                }
                (*polish)[num_of_elems][string_size - 1] = expr[i];
                ++string_size;
                ++i;
            } while (isdigit(expr[i]));
            (*polish)[num_of_elems][string_size - 1] = '\0';
            ++num_of_elems;
            --i;
            continue;
        }
        if (expr[i] == 'x') {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            if (!resize_buffer_string(&((*polish)[num_of_elems]), string_size, string_size + 2)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
//                if (num_of_elems > 0) { free(*polish); }
                free(*polish);
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > 0) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
            (*polish)[num_of_elems][0] = 'x';
            (*polish)[num_of_elems][1] = '\0';
            ++num_of_elems;
            continue;
        }
        if (strchr("&|=", expr[i]) != NULL && expr[i] != '\0') {
            if (!resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size + 1)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
                if (num_of_elems > 0) { free(*polish); }
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > -1) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
            if (!resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 3)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
                if (num_of_elems > 0) { free(*polish); }
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > -1) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
            functions[oper_arr_size][0] = expr[i];
            functions[oper_arr_size][1] = expr[i + 1];
            functions[oper_arr_size][2] = '\0';
            ++i;
            ++oper_arr_size;
            continue;
        }
        if (strchr("<>", expr[i]) != NULL && expr[i] != '\0') {
            if (!resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size + 1)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
                if (num_of_elems > 0) { free(*polish); }
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > -1) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
            if (expr[i + 1] == '=') {
                if (!resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 3)) {
                    for (int j = 0; j < num_of_elems; ++j) {
                        free((*polish)[j]);
                    }
                    if (num_of_elems > 0) { free(*polish); }
                    for (int j = 0; j < oper_arr_size; ++j) {
                        free(functions[j]);
                        ++j;
                    }
                    if (oper_arr_size > -1) { free(functions); }
                    perror("Allocation error!\n");
                    return -1;
                }
                functions[oper_arr_size][0] = expr[i];
                functions[oper_arr_size][1] = expr[i + 1];
                functions[oper_arr_size][2] = '\0';
                ++i;
            } else {
                if (!resize_buffer_string(&functions[oper_arr_size], string_size, string_size + 2)) {
                    for (int j = 0; j < num_of_elems; ++j) {
                        free((*polish)[j]);
                    }
                    if (num_of_elems > 0) { free(*polish); }
                    for (int j = 0; j < oper_arr_size; ++j) {
                        free(functions[j]);
                        ++j;
                    }
                    if (oper_arr_size > -1) { free(functions); }
                    perror("Allocation error!\n");
                    return -1;
                }
                functions[oper_arr_size][0] = expr[i];
                functions[oper_arr_size][1] = '\0';
            }
            ++oper_arr_size;
            continue;
        }
        if (expr[i] == ')' && oper_arr_size > 0) {
            resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1);
            if (!resize_buffer_string(&((*polish)[num_of_elems]), string_size,
                                      strlen(functions[oper_arr_size - 1]) + 1)) {
                for (int j = 0; j < num_of_elems; ++j) {
                    free((*polish)[j]);
                }
                if (num_of_elems > 0) { free(*polish); }
                for (int j = 0; j < oper_arr_size; ++j) {
                    free(functions[j]);
                    ++j;
                }
                if (oper_arr_size > 0) { free(functions); }
                perror("Allocation error!\n");
                return -1;
            }
//            strcpy((*polish)[num_of_elems], functions[oper_arr_size - 1]);
            memcpy((*polish)[num_of_elems], functions[oper_arr_size - 1],
                   strlen(functions[oper_arr_size - 1]) + 1);
            free(functions[oper_arr_size - 1]);
            resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size - 1);
            --oper_arr_size;
            ++num_of_elems;
            continue;
        }
        if (expr[i] == '\0') {
            while (functions) {
                string_size = 0;
                if (!resize_buffer_string_array(polish, num_of_elems, num_of_elems + 1)) {
                    for (int j = 0; j < num_of_elems; ++j) {
                        free((*polish)[j]);
                    }
                    if (num_of_elems > 0) { free(*polish); }
                    for (int j = 0; j < oper_arr_size; ++j) {
                        free(functions[j]);
                        ++j;
                    }
                    if (oper_arr_size > 0) { free(functions); }
                    perror("Allocation error!\n");
                    return -1;
                }
                if (!resize_buffer_string(&((*polish)[num_of_elems]), string_size,
                                          strlen(functions[oper_arr_size - 1]) + 1)) {
                    for (int j = 0; j < num_of_elems; ++j) {
                        free((*polish)[j]);
                    }
                    if (num_of_elems > 0) { free(*polish); }
                    for (int j = 0; j < oper_arr_size; ++j) {
                        free(functions[j]);
                        ++j;
                    }
                    if (oper_arr_size > 0) { free(functions); }
                    perror("Allocation error!\n");
                    return -1;
                }
//                strcpy((*polish)[num_of_elems], functions[oper_arr_size - 1], );
                memcpy((*polish)[num_of_elems], functions[oper_arr_size - 1],
                       strlen(functions[oper_arr_size - 1]) + 1);

                free(functions[oper_arr_size - 1]);
                resize_buffer_string_array(&functions, oper_arr_size, oper_arr_size - 1);
                --oper_arr_size;
                ++num_of_elems;
            }
        }
    }
    for (int i = 0; i < oper_arr_size; ++i) {
        free(functions[i]);
        ++i;
    }
//    if (oper_arr_size > 0) { free(functions); }
    free(functions);

    return num_of_elems;
}

static bool and_(bool a, bool b) { return a && b; }

static bool or_(bool a, bool b) { return a || b; }

static bool equal_(int a, int b) { return a == b; }

static bool lt_(int a, int b) { return a < b; }

static bool gt_(int a, int b) { return a > b; }

static bool let_(int a, int b) { return a <= b; }

static bool get_(int a, int b) { return a >= b; }

static int resize_buffer_ints(int **buffer, size_t old_length, size_t new_length) {
    // Allocating new buffer with check
    int *new_buffer = NULL;
    if (!(new_buffer = (int *) malloc(sizeof(int) * new_length))) {
        perror("Allocation error!");
        return 0;
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

    return 1;
}

static int use_operator(char *oper, int a, int b) {
    if (!strcmp(oper, "<")) { return lt_(a, b); }
    if (!strcmp(oper, "<=")) { return let_(a, b); }
    if (!strcmp(oper, ">")) { return gt_(a, b); }
    if (!strcmp(oper, ">=")) { return get_(a, b); }
    if (!strcmp(oper, "&&")) { return and_(a, b); }
    if (!strcmp(oper, "||")) { return or_(a, b); }
    if (!strcmp(oper, "==")) { return equal_(a, b); }
    perror("Operator identifying error!\n");
    return -1;
}

int pass_bool_expr(char **polish, int polish_size, int x) {
    int *results;
    int results_size = 0;
    for (int i = 0; i < polish_size; ++i) {
        if (isdigit(polish[i][0]) || polish[i][0] == '-') {
            char *p;
            if (!resize_buffer_ints(&results, results_size, results_size + 1)) {
                if (results_size > 0) { free(results); }
                perror("Decoding error!\n");
                return -1;
            }
            results[results_size] = (int) strtol(polish[i], &p, 10);
            ++results_size;
            continue;
        }
        if (!strcmp(polish[i], "x")) {
            if (!resize_buffer_ints(&results, results_size, results_size + 1)) {
                if (results_size > 0) { free(results); }
                perror("Decoding error!\n");
                return -1;
            }
            results[results_size] = x;
            ++results_size;
            continue;
        }

        int buf;
        if (results_size >= 2) {
            buf = use_operator(polish[i], results[results_size - 2], results[results_size - 1]);
        } else {
            if (results_size > 0) { free(results); }
            perror("Decoding error!\n");
            return -1;
        }
        resize_buffer_ints(&results, results_size, results_size - 1);
        --results_size;
        results[results_size - 1] = buf;
    }
    if (results_size != 1) {
        if (results_size > 1) { free(results); }
        perror("Decoding error!\n");
        return -1;
    }
    int res = results[0];
    free(results);
    return res;
}
