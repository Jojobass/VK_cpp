add_executable(size_1 size_1.cpp)
target_include_directories(size_1 PUBLIC ../../../include ../../../../polish_notation_lib/int_comparison/include)
target_link_libraries(size_1 PRIVATE bool_expr_parallel_lib polish_notation_int_comp_lib)

add_executable(size_2 size_2.cpp)
target_include_directories(size_2 PUBLIC ../../../include ../../../../polish_notation_lib/int_comparison/include)
target_link_libraries(size_2 PRIVATE bool_expr_parallel_lib polish_notation_int_comp_lib)

add_executable(size_10 size_10.cpp)
target_include_directories(size_10 PUBLIC ../../../include ../../../../polish_notation_lib/int_comparison/include)
target_link_libraries(size_10 PRIVATE bool_expr_parallel_lib polish_notation_int_comp_lib)