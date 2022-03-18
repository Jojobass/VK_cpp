#pragma once

#include <stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>

typedef struct {
    char *name;
    unsigned int price;
    unsigned int stock;
    char *manufacturer;
} entry;

int get_string(FILE *input_file, char **string, char delimiter);

void find_toys_from_country(entry cat[], int size, char const *country);

entry *prefill();

entry *new_entry(FILE *stream_);

entry *fill(int *size, FILE *stream_);
