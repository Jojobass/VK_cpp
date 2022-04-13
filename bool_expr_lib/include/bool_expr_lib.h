#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int get_num_passed(char *expr, int *arr, size_t arr_size);

int get_num_passed_(char *expr, int *arr, size_t arr_size, int processes);
