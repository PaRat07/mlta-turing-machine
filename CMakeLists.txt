cmake_minimum_required(VERSION 3.29)
project(turing-machine)

set(CMAKE_CXX_STANDARD 23)

set(REFLECTCPP_JSON OFF)
set(REFLECTCPP_YAML ON)

include(cmake/get_cpm.cmake)


CPMAddPackage("gh:jbeder/yaml-cpp#yaml-cpp-0.9.0")
CPMAddPackage("gh:getml/reflect-cpp#v0.24.0")

add_executable(turing-machine
        src/main.cpp
        src/parser.cpp
        src/interpreter.cpp
        src/config.cpp)

target_include_directories(turing-machine PRIVATE include)

target_link_libraries(turing-machine PRIVATE reflectcpp::reflectcpp)
