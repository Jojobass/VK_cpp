#include <gtest/gtest.h>
#include <ctime>

extern "C" {
#include <sys/mman.h>
#include <unistd.h>
#include <malloc.h>
#include "bool_expr_lib.h"
}

TEST(GET_NUM_PASSED, size_1) {
    int *arr = (int *) malloc(sizeof(int) * 1);
    char *expr = (char *) "x>1";
    arr[0] = 1;
    EXPECT_EQ(get_num_passed_(expr, arr, 1, (int) sysconf(_SC_NPROCESSORS_ONLN)), 0);
    arr[0] = 2;
    EXPECT_EQ(get_num_passed_(expr, arr, 1, (int) sysconf(_SC_NPROCESSORS_ONLN)), 1);
    free(arr);
}

TEST(GET_NUM_PASSED, size_2) {
    int *arr = (int *) malloc(sizeof(int) * 2);
    char *expr = (char *) "x>1";
    arr[0] = 1;
    arr[1] = 0;
    EXPECT_EQ(get_num_passed_(expr, arr, 2, (int) sysconf(_SC_NPROCESSORS_ONLN)), 0);
    arr[0] = 1;
    arr[1] = 2;
    EXPECT_EQ(get_num_passed_(expr, arr, 2, (int) sysconf(_SC_NPROCESSORS_ONLN)), 1);
    arr[0] = 2;
    arr[1] = 3;
    EXPECT_EQ(get_num_passed_(expr, arr, 2, (int) sysconf(_SC_NPROCESSORS_ONLN)), 2);
    free(arr);
}

TEST(GET_NUM_PASSED, size_10) {
    int *arr = (int *) malloc(sizeof(int) * 10);
    char *expr = (char *) "x>1";
    for (int i = 0; i < 10; ++i) {
        arr[i] = i - 4;
    }
    EXPECT_EQ(get_num_passed_(expr, arr, 10, (int) sysconf(_SC_NPROCESSORS_ONLN)), 4);
    free(arr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
