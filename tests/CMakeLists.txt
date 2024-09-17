cmake_minimum_required(VERSION 3.11)

project(cpp.tests)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)




set(APPLICATION_NAME cpp-tests)
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




add_executable (
    Cpp17
    Cpp17.cpp
)

target_link_libraries(
	Cpp17
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    CtorsDtors
    CtorsDtors.cpp
)

target_link_libraries(
    CtorsDtors
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    SmartPointers
    SmartPointers.cpp
)

target_link_libraries(
    SmartPointers
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    atomics
    atomics.cpp
)

target_link_libraries(
    atomics
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    sizeof
    sizeof.cpp
)

target_link_libraries(
    sizeof
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    std-move
    std-move.cpp
)

target_link_libraries(
    std-move
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)




add_executable (
    std-forward
    std-forward.cpp
)

target_link_libraries(
    std-forward
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)
