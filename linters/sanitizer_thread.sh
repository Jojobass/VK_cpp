#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

print_header "RUN sanitizers"

g++ -Wall -g -pthread -fsanitize=thread -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error polish_notation_lib/int_comparison/tests/int_comp_tests.cpp -Ipolish_notation_lib/int_comparison/include -lgtest_main -lgtest -lpthread -lpolish_notation_int_comp_lib -Lcmake-build-debug/polish_notation_lib/int_comparison -lgcov -o sanitizer_binary
check_log "./sanitizer_binary" "=ERROR"

g++ -Wall -g -pthread -fsanitize=thread -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error bool_expr_lib/successive/tests/successive_lib_tests.cpp -Ibool_expr_lib/include -Ipolish_notation_lib/int_comparison/include -lgtest_main -lgtest -lpthread -lbool_expr_successive_lib -Lcmake-build-debug/bool_expr_lib/successive -lpolish_notation_int_comp_lib -Lcmake-build-debug/polish_notation_lib/int_comparison -lgcov -o sanitizer_binary
check_log "./sanitizer_binary" "=ERROR"

g++ -Wall -g -pthread -fsanitize=thread -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error bool_expr_lib/parallel/tests/parallel_lib_test.cpp -Ibool_expr_lib/include -Ipolish_notation_lib/int_comparison/include -lgtest_main -lgtest -lpthread -lbool_expr_parallel_lib -Lcmake-build-debug/bool_expr_lib/parallel -lpolish_notation_int_comp_lib -Lcmake-build-debug/polish_notation_lib/int_comparison -lgcov -o sanitizer_binary
export LD_LIBRARY_PATH=cmake-build-debug/bool_expr_lib/parallel
check_log "./sanitizer_binary" "=ERROR"

print_header "SANITIZERS SUCCESS"