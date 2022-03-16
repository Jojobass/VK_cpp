#include "toys_catalogue.h"

int menu(FILE *stream_) {
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
      catalogue_size = 5;
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
  char *country = malloc(sizeof(char) * 50);
  fscanf(stream_, "%50[^\n]", country);
  fgetc(stream_);

  char *buf;
  if ((buf = realloc(country, strlen(country) * sizeof(char))) == NULL) {
    perror("Reallocation error!\n");
    free(country);
    return 1;
  } else {
    country = buf;
  }

  find_toys_from_country(catalogue, catalogue_size, country);

  free(country);
  free(catalogue);

  return 0;
}

int main() {
  setlocale(LC_ALL, "rus");

  return menu(stdin);
}
