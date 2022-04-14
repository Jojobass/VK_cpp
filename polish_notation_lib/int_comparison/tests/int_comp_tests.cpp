#include <gtest/gtest.h>
#include <ctime>

extern "C" {
#include "polish_int_comp_lib.h"
}

TEST(CHECK_VALID, operators_numbers_valid) {
    char *buf = (char *) "x>1";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "x<1";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "x>=1";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "x<=1";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "x==1";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
}

TEST(CHECK_VALID, spaces) {
    char *buf = (char *) " x > 1 ";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
}

TEST(CHECK_VALID, single_brackets) {
    char *buf = (char *) "(x>1)";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
}

TEST(CHECK_VALID, operators_boolean) {
    char *buf = (char *) "((x>1)&&(x<30))";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "((x>1)||(x<30))";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
}

TEST(CHECK_VALID, operators_numbers_invalid) {
    char *buf = (char *) "x>";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) ">x";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) ">t";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "t>";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "x>=";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "x=>1";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "(&&(x>1))";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "((x>1)&&)";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "(x>1)&&";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "((x>1)&|(x>30))";
    EXPECT_EQ(check_valid(&buf), 0);
}

TEST(CHECK_VALID, and_or_brackets) {
    char *buf = (char *) "((x>1)&&(x<30))";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "(((x>1)||(x<30))&&(x>-20))";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "((x>1)||((x<30)&&(x>-20)))";
    EXPECT_EQ(check_valid(&buf), 1);
    free(buf);
    buf = (char *) "((x>1)||(x<30)&&(x>-20))";
    EXPECT_EQ(check_valid(&buf), 0);
    buf = (char *) "(x<30)&&(x>-20)";
    EXPECT_EQ(check_valid(&buf), 0);
}

TEST(DETECT_FUNCTIONS, simplest) {
    char **test_polish;
    char *buf = (char *) "x>1";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(test_polish_size, 3);
    char *test_buf = (char *) (malloc(strlen(buf) + 1));
    strcpy(test_buf, test_polish[0]);
    for (int i = 1; i < test_polish_size; ++i) {
        strcat(test_buf, test_polish[i]);
    }
    EXPECT_STREQ(test_buf, "x1>");
    free(test_buf);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);

    buf = (char *) "(x>1)";
    test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(test_polish_size, 3);
    test_buf = (char *) malloc(strlen(buf) + 1);
    strcpy(test_buf, test_polish[0]);
    for (int i = 1; i < test_polish_size; ++i) {
        strcat(test_buf, test_polish[i]);
    }
    EXPECT_STREQ(test_buf, "x1>");
    free(test_buf);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}

TEST(DETECT_FUNCTIONS, single_and) {
    char **test_polish;
    char *buf = (char *) "((x>1)&&(x<10))";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(test_polish_size, 7);
    char *test_buf = (char *) malloc(strlen(buf) + 1);
    strcpy(test_buf, test_polish[0]);
    for (int i = 1; i < test_polish_size; ++i) {
        strcat(test_buf, test_polish[i]);
    }
    EXPECT_STREQ(test_buf, "x1>x10<&&");
    free(test_buf);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}

TEST(DETECT_FUNCTIONS, and_or) {
    char **test_polish;
    char *buf = (char *) "(((x>1)&&(x<10))||(x<-30))";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(test_polish_size, 11);
    char *test_buf = (char *) malloc(strlen(buf) + 1);
    strcpy(test_buf, test_polish[0]);
    for (int i = 1; i < test_polish_size; ++i) {
        strcat(test_buf, test_polish[i]);
    }
    EXPECT_STREQ(test_buf, "x1>x10<&&x-30<||");
    free(test_buf);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}

TEST(DETECT_FUNCTIONS, and_or_and) {
    char **test_polish;
    char *buf = (char *) "(((x>1)&&(x<10))||((x<-30)&&(x>-55)))";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(test_polish_size, 15);
    char *test_buf = (char *) malloc(strlen(buf) + 1);
    strcpy(test_buf, test_polish[0]);
    for (int i = 1; i < test_polish_size; ++i) {
        strcat(test_buf, test_polish[i]);
    }
    EXPECT_STREQ(test_buf, "x1>x10<&&x-30<x-55>&&||");
    free(test_buf);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}

TEST(PASS_BOOL_EXPR, simple) {
    char **test_polish;
    char *buf = (char *) "x>1";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 1), 0);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 2), 1);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}

TEST(PASS_BOOL_EXPR, complicated) {
    char **test_polish;
    char *buf = (char *) "(((x>1)&&(x<10))||((x<=-30)&&(x==-55)))";
    int test_polish_size = detect_functions(buf, &test_polish);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 1), 0);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 2), 1);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 9), 1);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, 10), 0);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, -30), 0);
    EXPECT_EQ(pass_bool_expr(test_polish, test_polish_size, -55), 1);
    for (int i = 0; i < test_polish_size; ++i) {
        free(test_polish[i]);
    }
    free(test_polish);
}
