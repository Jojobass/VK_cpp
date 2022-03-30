cmake_minimum_required(VERSION 3.16)
project(polish_notation_int_comp_lib)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fprofile-arcs -ftest-coverage -fPIC -O0")

file(GLOB SOURCES src/*.c)
file(GLOB INCLUDE_DIRS src)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

set(POLISH_NOTATION_INT_COMP_LIB_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(POLISH_NOTATION_INT_COMP_LIB_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)