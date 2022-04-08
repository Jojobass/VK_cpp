#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if ! echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

print_header "RUN valgrind [polish_notation_lib]"
check_log "valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all cmake-build-debug/polish_notation_lib/int_comparison/tests/test_int_comp_polish_notation" "no leaks are possible"

print_header "RUN valgrind [bool_expr_successive_lib]"
check_log "valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all cmake-build-debug/bool_expr_lib/successive/tests/test_bool_expr_successive" "no leaks are possible"

print_header "RUN valgrind [bool_expr_parallel_lib]"
check_log "valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all cmake-build-debug/bool_expr_lib/parallel/tests/valgrind_tests/size_1 cmake-build-debug/bool_expr_lib/parallel/tests/valgrind_tests/size_2 cmake-build-debug/bool_expr_lib/parallel/tests/valgrind_tests/size_10" "no leaks are possible"

print_header "VALGRIND SUCCESS"
