cmake_minimum_required(VERSION 3.29)
project(turing-machine-deps)

include(get_cpm.cmake)

CPMAddPackage("gh:jbeder/yaml-cpp#yaml-cpp-0.9.0")
CPMAddPackage("gh:getml/reflect-cpp#v0.24.0")
