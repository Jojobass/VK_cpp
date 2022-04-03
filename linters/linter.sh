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

print_header "RUN cppcheck"
check_log "cppcheck bool_expr_lib  polish_notation_lib main.c --enable=all --inconclusive --error-exitcode=1 --check-config --suppress=missingIncludeSystem --suppress=internalAstError --suppress=unmatchedSuppression -I polish_notation_lib/int_comparison/include -I bool_expr_lib/include" "\(information\)"

print_header "RUN clang-tidy"
check_log "clang-tidy bool_expr_lib/parallel/src/bool_expr_parallel.c bool_expr_lib/successive/src/bool_expr_successive.c  polish_notation_lib/int_comparison/src/int_comparison.c main.c -warnings-as-errors=* -extra-arg=-std=gnu99 -- -Ibool_expr_lib/include -Ipolish_notation_lib/int_comparison/include" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --extensions=c bool_expr_lib/parallel/src/* bool_expr_lib/include/* bool_expr_lib/successive/src/* polish_notation_lib/int_comparison/src/* polish_notation_lib/int_comparison/include/* main.c" "Can't open for reading"

print_header "LINTERS SUCCESS"
