cmake_minimum_required(VERSION 3.11)

project(square_solver)
set(APPLICATION_NAME square_solver)
set (CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

file(GLOB APPLICATION_SOURCES *.cpp)

add_executable (
    ${APPLICATION_NAME}
    ${APPLICATION_SOURCES}
)

target_link_libraries(
    ${APPLICATION_NAME}
    Threads::Threads
)

