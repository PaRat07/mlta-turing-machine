cmake_minimum_required(VERSION 3.29)
project(pm)

set(CMAKE_CXX_STANDARD 20)

set(REFLECTCPP_JSON OFF)
set(REFLECTCPP_YAML ON)

include(cmake/get_cpm.cmake)


CPMAddPackage("gh:jbeder/yaml-cpp#yaml-cpp-0.9.0")
CPMAddPackage("gh:getml/reflect-cpp#v0.24.0")

add_executable(pm src/main.cpp)

target_link_libraries(pm PRIVATE reflectcpp::reflectcpp)
