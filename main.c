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

void find_toys_from_country(entry cat[], size_t size, char *country) {
    int in_stock = 0, out_of_stock = 0;
    for (int i = 0; i < size; ++i) {
        if (!strcmp(cat[i].manufacturer, country) && cat[i].stock > 0) {
            if (in_stock == 0) { printf("В наличии:\n"); }
            printf("%s, %d Руб., %s\n", cat[i].name, cat[i].price, cat[i].manufacturer);
            ++in_stock;
        }
    }
    for (int i = 0; i < size; ++i) {
        if (!strcmp(cat[i].manufacturer, country) && cat[i].stock == 0) {
            if (out_of_stock == 0) { printf("Нет в наличии:\n"); }
            printf("%s, %d Руб., %s, %d шт.\n", cat[i].name, cat[i].price, cat[i].manufacturer, cat[i].stock);
            ++out_of_stock;
        }
    }
    if(!in_stock && !out_of_stock){ printf("По запросу ничего не найдено\n");}
}

entry *prefill() {
    entry *catalogue = malloc(sizeof(entry) * 5);
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

entry *new_entry() {
    entry* new_entry_ = malloc(sizeof(entry));

    printf("Введите название товара: ");
    new_entry_->name = malloc(sizeof(char)*100);
    fscanf(stdin, "%100[^\n]", new_entry_->name);
    getchar();

    char* buf;
    if((buf = realloc(new_entry_->name, strlen(new_entry_->name) * sizeof(char))) == NULL){
        perror("Reallocation error!\n");
        free(new_entry_->name);
        return NULL;
    } else {
        new_entry_->name = buf;
    }

    printf("Введите стоимость: ");
    fscanf(stdin, "%ud", &new_entry_->price);
    getchar();

    printf("Введите число товаров в наличии: ");
    fscanf(stdin, "%ud", &new_entry_->stock);
    getchar();

    printf("Введите страну производства: ");
    new_entry_->manufacturer = malloc(sizeof(char)*50);
    fscanf(stdin, "%50[^\n]", new_entry_->manufacturer);
    getchar();

    if((buf = realloc(new_entry_->manufacturer, strlen(new_entry_->manufacturer) * sizeof(char))) == NULL){
        perror("Reallocation error!\n");
        free(new_entry_->manufacturer);
        return NULL;
    } else {
        new_entry_->manufacturer = buf;
    }

    return new_entry_;
}

entry *fill(int* size) {
    entry *catalogue = malloc(sizeof(entry));

    int cnt = 0;
    char y_n = 'y';
    do {
        entry* buf;
        if((buf = new_entry()) == NULL){
            return NULL;
        }else {
            catalogue[cnt] = *buf;
        }

        do {
            printf("Добавить еще одну запись? (y/n)\n");
            y_n = fgetc(stdin);
            getchar();
        } while (y_n != 'y' && y_n != 'n');

        ++cnt;

        if(y_n == 'y'){
            entry* buf;

            if((buf = realloc(catalogue, sizeof(entry)*(cnt+1))) == NULL){
                perror("Reallocation error!\n");
                free(catalogue);
                return NULL;
            } else {
                catalogue = buf;
            }
        }
    } while (y_n == 'y');
    *size = cnt;
    return catalogue;
}

int main() {
    setlocale(LC_ALL, "rus");

    int choice = 0;
    entry *catalogue;
    int catalogue_size = 0;

    do{
        printf("Выберите:\n(1) Использовать существующий каталог\n(2) Ввести каталог\n");
        fscanf(stdin, "%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                catalogue = prefill();
                catalogue_size = 5;
                break;
            case 2:
                if((catalogue = fill(&catalogue_size)) == NULL) return 1;
                break;
            default:
                break;
        }
    }while(choice != 1 && choice !=2);


    printf("Введите название страны для поиска:\n");
    char *country = malloc(sizeof(char)*50);
    fscanf(stdin, "%50[^\n]", country);
    getchar();

    char* buf;
    if((buf = realloc(country, strlen(country) * sizeof(char))) == NULL){
        perror("Reallocation error!\n");
        free(country);
        return 1;
    } else {
        country = buf;
    }

    find_toys_from_country(catalogue, catalogue_size, country);

    for(int i=0; i<catalogue_size; ++i){
        free(catalogue[i].name);
        free(catalogue[i].manufacturer);
    }
    free(catalogue);
}
