cmake_minimum_required(VERSION 3.16)
project(toys_search)
set(CMAKE_C_STANDARD 99)

add_subdirectory(toys_catalogue_lib)

message("CAT_LIB_INCLUDE_DIRS = ${CAT_LIB_INCLUDE_DIRS}")
message("CAT_LIB_LIBRARIES = ${CAT_LIB_LIBRARIES}")

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_C_FLAGS "-fprofile-arcs -ftest-coverage -fPIC -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${CAT_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CAT_LIB_LIBRARIES})

add_subdirectory(tests)