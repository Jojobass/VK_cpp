cmake_minimum_required(VERSION 3.16)
project(test_toys_search)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

file(GLOB SOURCES *.cpp)

#find_package(GTest REQUIRED)
#add_subdirectory("/usr/src/googletest" ${CMAKE_BINARY_DIR}/gtest)
#include_directories(${GTEST_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929
)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CAT_LIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${CAT_LIB_LIBRARIES} GTest::gtest_main)