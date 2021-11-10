cmake_minimum_required(VERSION 3.11)

project(provider_supplier)
set(APPLICATION_NAME provider_supplier)
set(CMAKE_CXX_STANDARD 14)

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

