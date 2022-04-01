add_library(test_stress_parallel src/test_parallel.c)
target_link_libraries(test_stress_parallel PRIVATE bool_expr_parallel_lib ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})
target_include_directories(test_stress_parallel PUBLIC include ../../bool_expr_lib/parallel/include)

add_library(test_stress_successive src/test_successive.c)
target_link_libraries(test_stress_successive PRIVATE bool_expr_successive_lib ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})
target_include_directories(test_stress_successive PUBLIC include ../../bool_expr_lib/successive/include)
