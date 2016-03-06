add_executable (test_${PROJECT_NAME} main.cpp minmax.cpp modifying_seq_ops.cpp non_modifying_seq_ops.cpp numeric.cpp set_ops.cpp)
ADD_TESTINATOR_TESTS (test_${PROJECT_NAME})
