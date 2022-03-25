#include "toys_catalogue.h"

#define PREFILL_SIZE 5

int menu(FILE *stream_) {
    if (stream_ == NULL) {
        perror("Strean is NULL!");
        return 1;
    }
    int choice = 0;
    entry *catalogue;
    int catalogue_size = 0;

    do {
        printf("Выберите:\n(1) Использовать существующий каталог\n(2) Ввести "
               "каталог\n");
        fscanf(stream_, "%d", &choice);
        fgetc(stream_);

        switch (choice) {
            case 1:
                catalogue = prefill();
                catalogue_size = PREFILL_SIZE;
                break;
            case 2:
                if ((catalogue = fill(&catalogue_size, stream_)) == NULL)
                    return 1;
                break;
            default:
                break;
        }
    } while (choice != 1 && choice != 2);

    printf("Введите название страны для поиска:\n");
    char *country;
    if (get_string(stream_, &country, '\n')) {
        return 1;
    }

    find_toys_from_country(catalogue, catalogue_size, country);

    free(country);
    if (choice == 2) {
        for (int i = 0; i < catalogue_size; ++i) {
            free(catalogue[i].name);
            free(catalogue[i].manufacturer);
        }
    }
    free(catalogue);

    return 0;
}

int main() {
    setlocale(LC_ALL, "rus");

    return menu(stdin);
}
