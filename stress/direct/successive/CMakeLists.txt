add_library(test_stress_successive src/test_successive.c)
target_link_libraries(test_stress_successive PUBLIC bool_expr_successive_lib ${POLISH_NOTATION_INT_COMP_LIB_LIBRARIES})
target_include_directories(test_stress_successive PUBLIC include ../../../bool_expr_lib/include)
