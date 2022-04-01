cmake_minimum_required(VERSION 3.16)
project(bool_expr_parallel_lib)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fprofile-arcs -ftest-coverage -fPIC -O0")

file(GLOB SOURCES src/*.c)
file(GLOB INCLUDE_DIRS ../include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_include_directories(${PROJECT_NAME} PUBLIC ${POLISH_NOTATION_INT_COMP_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})

add_subdirectory(tests)

set(BOOL_EXPR_PARALLEL_LIB_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
set(BOOL_EXPR_PARALLEL_LIB_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)