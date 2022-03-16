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
check_log "cppcheck toys_catalogue_lib main.c --enable=all --inconclusive --error-exitcode=1  --suppress=missingIncludeSystem -I toys_catalogue_lib" "\(information\)"

print_header "RUN clang-tidy"
check_log "clang-tidy toys_catalogue_lib/toys_catalogue.c toys_catalogue_lib/toys_catalogue.h main.c -warnings-as-errors=* -extra-arg=-std=c99 -- -Itoys_catalogue_lib" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --extensions=c toys_catalogue_lib/* main.c" "Can't open for reading"

print_header "LINTERS SUCCESS"

print_header "RUN valgrind"
check_log "valgrind --tool=memcheck --leak-check=yes cmake-build-debug/tests/test_toys_search" ""
