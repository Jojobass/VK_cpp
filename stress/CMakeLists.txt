set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-fprofile-arcs -ftest-coverage -fPIC -O0")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -lgcov" )

add_subdirectory(direct/successive)
add_subdirectory(direct/parallel)

add_executable(stress_test stress.cpp)
target_link_libraries(stress_test test_stress_successive test_stress_parallel)
#target_include_directories(stress_test PUBLIC direct/include)
