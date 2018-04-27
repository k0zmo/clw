cmake_minimum_required(VERSION 3.9)
project(clw CXX)

find_package(OpenCL REQUIRED)

option(CLW_ENABLE_OPENCL_1_2 "Enable OpenCL 1.2 features" OFF)

add_subdirectory(clw)
add_subdirectory(clwinfo)
