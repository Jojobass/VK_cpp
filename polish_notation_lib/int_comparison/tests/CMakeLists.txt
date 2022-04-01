cmake_minimum_required(VERSION 3.16)
project(test_int_comp_polish_notation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fprofile-arcs -ftest-coverage -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

file(GLOB SOURCES *.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929
)
FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_link_libraries(${PROJECT_NAME} PRIVATE polish_notation_int_comp_lib GTest::gtest_main)


#target_include_directories(${PROJECT_NAME} PUBLIC ${POLISH_NOTATION_INT_COMP_LIB_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES} GTest::gtest_main)
