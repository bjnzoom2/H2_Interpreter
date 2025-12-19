cmake_minimum_required(VERSION 4.1)
project(H2_Interpreter)

set(CMAKE_CXX_STANDARD 20)

add_executable(H2_Interpreter main.cpp)
configure_file(File.h2 ${CMAKE_CURRENT_BINARY_DIR}/File.h2 COPYONLY)