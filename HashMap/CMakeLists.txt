cmake_minimum_required(VERSION 3.11)

project(HashMap)
set(APPLICATION_NAME HashMap)
set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_DEBUG_POSTFIX d)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

file(GLOB APPLICATION_SOURCES *.cpp)

add_executable (
    ${APPLICATION_NAME}
    ${APPLICATION_SOURCES}
)
set_target_properties(${APPLICATION_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(
    ${APPLICATION_NAME}
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)

