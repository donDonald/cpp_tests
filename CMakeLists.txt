cmake_minimum_required(VERSION 3.11)
project(cpp_tests VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(BTree)
add_subdirectory(HashMap)
add_subdirectory(ThreadingTest)
add_subdirectory(FalseSharing)
add_subdirectory(provider_consumer)
add_subdirectory(square_solver)
add_subdirectory(tests)
add_subdirectory(algorithms)

