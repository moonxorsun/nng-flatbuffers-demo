cmake_minimum_required(VERSION 3.12)

project(nng-flatbuffers-demo VERSION 0.0.1 LANGUAGES C CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/utils "*.cpp")

add_executable(test_nng test_nng.cpp)

list(APPEND test_nng_libs nng)

target_link_libraries(test_nng ${test_nng_libs})
