cmake_minimum_required(VERSION 3.16)
project(C_HW_2)

set(CMAKE_C_STANDARD 11)

add_subdirectory(bool_expr_lib/parallel)
add_subdirectory(bool_expr_lib/successive)
add_subdirectory(polish_notation_lib/int_comparison)
add_subdirectory(stress)

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage -fPIC -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -lgcov" )

option(WITH_PARALLEL "Using shared library" ON)
if(WITH_PARALLEL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${BOOL_EXPR_PARALLEL_LIB_LIBRARIES} ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${BOOL_EXPR_SUCCESSIVE_LIB_LIBRARIES} ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})
endif()
