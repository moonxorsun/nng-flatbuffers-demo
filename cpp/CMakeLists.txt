cmake_minimum_required(VERSION 3.12)

project(nng-flatbuffers-demo VERSION 0.0.1 LANGUAGES C CXX)

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIC -O3 -lpthread") # -fvisibility=hidden
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -std=c++11 -lpthread")

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#link_libraries(pthread)
#add_compile_options(-lpthread)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/utils "*.c")

add_executable(test_nng test_nng.cpp)
add_executable(test_demo test_demo.cpp)

list(APPEND test_nng_libs nng)

target_link_libraries(test_nng ${test_nng_libs})
target_link_libraries(test_demo ${test_nng_libs})
