cmake_minimum_required(VERSION 3.11)

project(parralel_parsing)
set(APPLICATION_NAME parralel_parsing)
set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

set(Boost_USE_MULTITHREAD ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)
find_package(Boost 1.70.0 REQUIRED COMPONENTS system thread program_options)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})




add_executable (
    parser
    parser.cpp
)

target_link_libraries(
    parser
    ${GTEST_BOTH_LIBRARIES}
	${Boost_LIBRARIES}
    Threads::Threads
)




add_executable (
    generator
    generator.cpp
)

target_link_libraries(
    generator
    ${GTEST_BOTH_LIBRARIES}
	${Boost_LIBRARIES}
    Threads::Threads
)
