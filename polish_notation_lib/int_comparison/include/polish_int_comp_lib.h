#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>

bool check_valid(char **expr_);
int detect_functions(char *expr, char ***polish);
