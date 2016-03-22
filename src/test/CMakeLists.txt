add_executable (test_${PROJECT_NAME} heap_ops.cpp main.cpp minmax.cpp modifying_seq_ops.cpp non_modifying_seq_ops.cpp numeric.cpp partitioning_ops.cpp set_ops.cpp sort_ops.cpp)
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})
