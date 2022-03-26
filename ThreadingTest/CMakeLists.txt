cmake_minimum_required(VERSION 3.11)

project(ThreadingTest)
set(APPLICATION_NAME ThreadingTest)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

file(GLOB APPLICATION_SOURCES *.cpp)

add_executable (
    ${APPLICATION_NAME}
    ${APPLICATION_SOURCES}
)

target_link_libraries(
    ${APPLICATION_NAME}
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)

