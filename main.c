#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include "bool_expr_lib.h"
// #include "bool_expr_successive_lib.h"

int resize_buffer_ints(int **buffer, size_t old_length, size_t new_length) {
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

int read_file(FILE *stream_, int **arr) {
    int arr_length = 0;
    while (!feof(stream_)) {
        resize_buffer_ints(arr, arr_length, arr_length + 1);
        fscanf(stream_, "%d\n", &((*arr)[arr_length]));
        ++arr_length;
    }
    return arr_length;
}

int resize_buffer_string(char **buffer, size_t old_length, size_t new_length) {
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

int get_string(FILE *input_file, char **string, char delimiter) {
    if (resize_buffer_string(string, 0, 1) != 1) {
        free(*string);
        return 1;
    }

    size_t buffer_length = 1;

    char current_char = (char) fgetc(input_file);
    while (current_char != delimiter) {
        if (resize_buffer_string(string, buffer_length, buffer_length + 1) != 1) {
            free(*string);
            return 1;
        }
        (*string)[buffer_length - 1] = current_char;
        ++buffer_length;


        current_char = (char) fgetc(input_file);
    }

    (*string)[buffer_length - 1] = '\0';

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Choose a preferred setup:\n1 - automatic\n2 - manual\n");
        int ch = 0;
        scanf("%d", &ch);
        fgetc(stdin);
        if (ch == 1) {
            int *big_arr = malloc(100000000 * sizeof(int));
            unsigned int seed = time(NULL);
            for (size_t i = 0; i < 100000000; ++i) {
                big_arr[i] = rand_r(&seed) % 100 + 1;
            }
            int probe[1] = {1};
            if (get_num_passed("((70 < x) && (x <= 90))", probe, 1) != -2) {
                printf("%d", get_num_passed("((70 < x) && (x <= 90))", big_arr, 100000000));
            } else {
                printf("%d", get_num_passed_("((70 < x) && (x <= 90))", big_arr, 100000000));
            }

            free(big_arr);
            return 0;
        }
        if (ch == 2) {
            printf("Please enter an expression, for which x between 1 and 100 may give an expected result\n");
            char *expr;
            get_string(stdin, &expr, '\n');

            int *big_arr = malloc(100000000 * sizeof(int));
            unsigned int seed = time(NULL);
            for (size_t i = 0; i < 100000000; ++i) {
                big_arr[i] = rand_r(&seed) % 100 + 1;
            }
            int probe[1] = {1};
            if (get_num_passed(expr, probe, 1) != -2) {
                printf("%d", get_num_passed(expr, big_arr, 100000000));
            } else {
                printf("%d", get_num_passed_(expr, big_arr, 100000000));
            }

            free(big_arr);
            return 0;
        }
        return 1;
    }
    if (argc == 3) {
        FILE *stream = fopen(argv[2], "r");
        int *big_arr;
        int arr_size = read_file(stream, &big_arr);
        char *expr = argv[1];
        int probe[1] = {1};
        if (get_num_passed(expr, probe, 1) != -2) {
            printf("%d", get_num_passed(expr, big_arr, arr_size));
        } else {
            printf("%d", get_num_passed_(expr, big_arr, arr_size));
        }
    }

    return 0;
}
