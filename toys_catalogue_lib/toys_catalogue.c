#include "toys_catalogue.h"

#define DEFAULT_INITIAL_LENGTH 4

int static grow_buffer(char **buffer, size_t old_length, size_t new_length) {
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

int get_string(FILE *input_file, char **string, char delimiter) {
    //  Начальная аллокация
    if (grow_buffer(string, 0, DEFAULT_INITIAL_LENGTH) != 0) {
        return 1;
    }
    size_t buffer_length = DEFAULT_INITIAL_LENGTH;

    size_t string_length = 0;

    char current_char = (char) fgetc(input_file);
    while (current_char != delimiter) {
        (*string)[string_length] = current_char;
        ++string_length;

        if (string_length == buffer_length) {
            if (grow_buffer(string, buffer_length, buffer_length * 2) != 0) {
                free(*string);
                return 1;
            }
            buffer_length *= 2;
        }

        current_char = (char) fgetc(input_file);
    }

    (*string)[string_length] = '\0';

    return 0;
}

void find_toys_from_country(entry cat[], int size, char const *country) {
    int in_stock = 0, out_of_stock = 0;
    for (int i = 0; i < size; ++i) {
        if (!strcmp(cat[i].manufacturer, country) && cat[i].stock > 0) {
            if (in_stock == 0) { printf("В наличии:\n"); }
            printf("%s, %u Руб., %s, %u шт.\n",
                   cat[i].name, cat[i].price, cat[i].manufacturer, cat[i].stock);
            ++in_stock;
        }
    }
    for (int i = 0; i < size; ++i) {
        if (!strcmp(cat[i].manufacturer, country) && cat[i].stock == 0) {
            if (out_of_stock == 0) { printf("Нет в наличии:\n"); }
            printf("%s, %u Руб., %s, %u шт.\n",
                   cat[i].name, cat[i].price, cat[i].manufacturer, cat[i].stock);
            ++out_of_stock;
        }
    }
    if (!in_stock && !out_of_stock) { printf("По запросу ничего не найдено\n"); }
}

entry *prefill() {
    entry *catalogue;
    if ((catalogue = malloc(sizeof(entry) * 5)) == NULL) {
        perror("Allocation error!");
        return NULL;
    }

    {
        catalogue[0].name = "LEGO City 123956";
        catalogue[0].price = 1560;
        catalogue[0].stock = 17;
        catalogue[0].manufacturer = "Дания";
    }
    {
        catalogue[1].name = "Barbie Policewoman";
        catalogue[1].price = 890;
        catalogue[1].stock = 7;
        catalogue[1].manufacturer = "США";
    }
    {
        catalogue[2].name = "NERF Blaster 3000";
        catalogue[2].price = 2250;
        catalogue[2].stock = 5;
        catalogue[2].manufacturer = "США";
    }
    {
        catalogue[3].name = "Tamagotchi Blastoise Evo+";
        catalogue[3].price = 560;
        catalogue[3].stock = 15;
        catalogue[3].manufacturer = "Япония";
    }
    {
        catalogue[4].name = "Gundam Universe Mobile Suit Gundam XXXG-01h Gundam Heavy Arms";
        catalogue[4].price = 3600;
        catalogue[4].stock = 0;
        catalogue[4].manufacturer = "Япония";
    }

    return catalogue;
}

entry *new_entry(FILE *stream_) {
    entry *new_entry_;
    if ((new_entry_ = malloc(sizeof(entry))) == NULL) {
        perror("Allocation error!");
        return NULL;
    }

    printf("Введите название товара: ");
    if (get_string(stream_, &new_entry_->name, '\n')) {
        free(new_entry_);
        return NULL;
    }

    printf("Введите стоимость: ");
    fscanf(stream_, "%ud", &new_entry_->price);
    fgetc(stream_);

    printf("Введите число товаров в наличии: ");
    fscanf(stream_, "%ud", &new_entry_->stock);
    fgetc(stream_);

    printf("Введите страну производства: ");
    if (get_string(stream_, &new_entry_->manufacturer, '\n')) {
        free(new_entry_->name);
        free(new_entry_);
        return NULL;
    }

    return new_entry_;
}

entry *fill(int *size, FILE *stream_) {
    entry *catalogue;
    if ((catalogue = malloc(sizeof(entry))) == NULL) {
        perror("Allocation error!");
        return NULL;
    }

    int cnt = 0;
    char y_n;
    do {
        entry *buf;
        if ((buf = new_entry(stream_)) == NULL) {
            for (int i = 0; i < cnt; ++i) {
                free(catalogue[i].name);
                free(catalogue[i].manufacturer);
            }
            free(catalogue);
            free(buf);
            return NULL;
        } else {
            catalogue[cnt] = *buf;
        }
        free(buf);

        do {
            printf("Добавить еще одну запись? (y/n)\n");
            y_n = (char) fgetc(stream_);
            fgetc(stream_);
        } while (y_n != 'y' && y_n != 'n');

        ++cnt;

        if (y_n == 'y') {
            entry *buf1;

            if ((buf1 = realloc(catalogue, sizeof(entry) * (cnt + 1))) == NULL) {
                perror("Reallocation error!\n");
                for (int i = 0; i <= cnt; ++i) {
                    free(catalogue[i].name);
                    free(catalogue[i].manufacturer);
                }
                free(catalogue);
                free(buf1);
                return NULL;
            } else {
                catalogue = buf1;
            }
        }
    } while (y_n == 'y');

    *size = cnt;
    return catalogue;
}
