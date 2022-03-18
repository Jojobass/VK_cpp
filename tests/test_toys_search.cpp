#include <gtest/gtest.h>
#include<string>

extern "C" {
#include "toys_catalogue.h"
}

TEST(GetString, length_0) {
    char *input_str = (char *) "\n";
    FILE *input = fmemopen(input_str, strlen(input_str), "r");
    char *output;
    char *test = (char *) "\0";
    if (!get_string(input, &output, '\n')) {
        EXPECT_STREQ(output, test);
        free(output);
    }
    fclose(input);
}

TEST(GetString, length_3) {
    char *input_str = (char *) "Ab1\n";
    FILE *input = fmemopen(input_str, strlen(input_str), "r");
    char *output;
    char *test = (char *) "Ab1\0";
    if (!get_string(input, &output, '\n')) {
        EXPECT_STREQ(output, test);
        free(output);
    }
    fclose(input);
}

TEST(GetString, length_4) {
    char *input_str = (char *) "Ab1 \n";
    FILE *input = fmemopen(input_str, strlen(input_str), "r");
    char *output;
    char *test = (char *) "Ab1 \0";
    if (!get_string(input, &output, '\n')) {
        EXPECT_STREQ(output, test);
        free(output);
    }
    fclose(input);
}

TEST(GetString, length_8) {
    char *input_str = (char *) "Ab1 _!\\ \n";
    FILE *input = fmemopen(input_str, strlen(input_str), "r");
    char *output;
    char *test = (char *) "Ab1 _!\\ \0";
    if (!get_string(input, &output, '\n')) {
        EXPECT_STREQ(output, test);
        free(output);
    }
    fclose(input);
}

TEST(Search, find_0) {
    entry *catalogue = prefill();
    int size = 5;
    char *input_str = (char *) "Германия";

    testing::internal::CaptureStdout();
    find_toys_from_country(catalogue, size, input_str);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), "По запросу ничего не найдено\n");
    free(catalogue);
}

TEST(Search, find_1_instock) {
    entry *catalogue = prefill();
    int size = 5;
    char *input_str = (char *) "Дания";
    testing::internal::CaptureStdout();
    find_toys_from_country(catalogue, size, input_str);
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "В наличии:\nLEGO City 123956, 1560 Руб., Дания, 17 шт.\n");
    free(catalogue);
}

TEST(Search, find_2_instock) {
    entry *catalogue = prefill();
    int size = 5;
    char *input_str = (char *) "США";
    testing::internal::CaptureStdout();
    find_toys_from_country(catalogue, size, input_str);
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "В наличии:\nBarbie Policewoman, 890 Руб., США, 7 шт.\nNERF Blaster 3000, 2250 Руб., США, 5 шт.\n");
    free(catalogue);
}

TEST(Search, find_1_instock_1_outstock) {
    entry *catalogue = prefill();
    int size = 5;
    char *input_str = (char *) "Япония";
    testing::internal::CaptureStdout();
    find_toys_from_country(catalogue, size, input_str);
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "В наличии:\nTamagotchi Blastoise Evo+, 560 Руб., Япония, 15 шт.\nНет в наличии:\nGundam Universe Mobile Suit Gundam XXXG-01h Gundam Heavy Arms, 3600 Руб., Япония, 0 шт.\n");
    free(catalogue);
}

TEST(New_entry, eng) {
    char *input_str_1 = (char *) "Test product\n1000\n10\nTest country\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *buf;
    if ((buf = new_entry(input)) == NULL) {
        fclose(input);
    } else {
        entry *new_entry_ = buf;
        entry test = {(char *) "Test product", 1000, 10, (char *) "Test country"};
        EXPECT_STREQ(test.name, new_entry_->name);
        EXPECT_EQ(test.price, new_entry_->price);
        EXPECT_EQ(test.stock, new_entry_->stock);
        EXPECT_STREQ(test.manufacturer, new_entry_->manufacturer);
        free(new_entry_->name);
        free(new_entry_->manufacturer);
        free(new_entry_);
        fclose(input);
    }
}

TEST(New_entry, rus) {
    char *input_str_1 = (char *) "Тестовая игрушка\n1000\n10\nТестовая страна\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *buf;
    if ((buf = new_entry(input)) == NULL) {
        fclose(input);
    } else {
        entry *new_entry_ = buf;
        entry test = {(char *) "Тестовая игрушка", 1000, 10, (char *) "Тестовая страна"};
        EXPECT_STREQ(test.name, new_entry_->name);
        EXPECT_EQ(test.price, new_entry_->price);
        EXPECT_EQ(test.stock, new_entry_->stock);
        EXPECT_STREQ(test.manufacturer, new_entry_->manufacturer);
        free(new_entry_->name);
        free(new_entry_->manufacturer);
        free(new_entry_);
        fclose(input);
    }
}

TEST(Fill, fill) {
    int size = 0;
    char *input_str_1 = (char *) "Test product\n1000\n10\nTest country\nn\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *catalogue;
    if ((catalogue = fill(&size, input)) == NULL) {
        fclose(input);
    } else {
        entry test = {(char *) "Test product", 1000, 10, (char *) "Test country"};
        EXPECT_STREQ(test.name, catalogue->name);
        EXPECT_EQ(test.price, catalogue->price);
        EXPECT_EQ(test.stock, catalogue->stock);
        EXPECT_STREQ(test.manufacturer, catalogue->manufacturer);
        free(catalogue->name);
        free(catalogue->manufacturer);
        free(catalogue);
        fclose(input);
    }
}

TEST(Fill, fill_1_find_1) {
    int size = 0;
    char *input_str_1 = (char *) "Test product\n1000\n10\nTest country\nn\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *catalogue;
    if ((catalogue = fill(&size, input)) == NULL) {
        fclose(input);
    } else {
        char *input_str_2 = (char *) "Test country";
        testing::internal::CaptureStdout();
        find_toys_from_country(catalogue, size, input_str_2);
        EXPECT_EQ(testing::internal::GetCapturedStdout(),
                  "В наличии:\nTest product, 1000 Руб., Test country, 10 шт.\n");
        free(catalogue->name);
        free(catalogue->manufacturer);
        free(catalogue);
        fclose(input);
    }
}

TEST(Fill, fill_2_find_1) {
    int size = 0;
    char *input_str_1 = (char *) "Test product 1\n1000\n10\nTest country 1\ny\nTest product 2\n2000\n20\nTest country 2\nn\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *catalogue;
    if ((catalogue = fill(&size, input)) == NULL) {
        fclose(input);
    } else {
        char *input_str_2 = (char *) "Test country 1";
        testing::internal::CaptureStdout();
        find_toys_from_country(catalogue, size, input_str_2);
        EXPECT_EQ(testing::internal::GetCapturedStdout(),
                  "В наличии:\nTest product 1, 1000 Руб., Test country 1, 10 шт.\n");
        for (int i = 0; i < size; ++i) {
            free(catalogue[i].name);
            free(catalogue[i].manufacturer);
        }
        free(catalogue);
        fclose(input);
    }
}

TEST(Fill, fill_2_find_2) {
    int size = 0;
    char *input_str_1 = (char *) "Test product 1\n1000\n10\nTest country 1\ny\nTest product 2\n2000\n0\nTest country 1\nn\n";
    FILE *input = fmemopen(input_str_1, strlen(input_str_1), "r");
    entry *catalogue;
    if ((catalogue = fill(&size, input)) == NULL) {
        fclose(input);
    } else {
        char *input_str_2 = (char *) "Test country 1";
        testing::internal::CaptureStdout();
        find_toys_from_country(catalogue, size, input_str_2);
        EXPECT_EQ(testing::internal::GetCapturedStdout(),
                  "В наличии:\nTest product 1, 1000 Руб., Test country 1, 10 шт.\nНет в наличии:\nTest product 2, 2000 Руб., Test country 1, 0 шт.\n");
        for (int i = 0; i < size; ++i) {
            free(catalogue[i].name);
            free(catalogue[i].manufacturer);
        }
        free(catalogue);
        fclose(input);
    }
}