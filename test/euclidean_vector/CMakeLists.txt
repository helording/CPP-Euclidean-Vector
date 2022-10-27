cxx_test(
   TARGET euclidean_vector_constructor_tests
   FILENAME "euclidean_vector_constructors_tests.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_getter_setters_tests
   FILENAME "euclidean_vector_getters_setters_tests.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_member_function_tests
   FILENAME "euclidean_vector_member_functions_tests.cpp"
   LINK euclidean_vector
)

cxx_test(
   TARGET euclidean_vector_friend_function_tests
   FILENAME "euclidean_vector_friend_functions_tests.cpp"
   LINK euclidean_vector
)
