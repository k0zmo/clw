cmake_minimum_required(VERSION 3.9)
project(clw LANGUAGES CXX VERSION 0.1)

find_package(OpenCL REQUIRED)

option(CLW_ENABLE_OPENCL_1_2 "Enable OpenCL 1.2 features" ON)

add_subdirectory(src)
add_subdirectory(tool)

# Installation stuff
set(config_file ${CMAKE_CURRENT_BINARY_DIR}/clwConfig.cmake)
set(config_version_file ${CMAKE_CURRENT_BINARY_DIR}/clwConfigVersion.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${config_version_file}
                                 VERSION ${clw_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

file(WRITE ${config_file}
"include(CMakeFindDependencyMacro)
find_dependency(OpenCL)
if(NOT TARGET clw::clw)
  include(\"\${CMAKE_CURRENT_LIST_DIR}/clwTarget.cmake\")
endif()
")

install(FILES ${config_file} ${config_version_file}
    DESTINATION lib/cmake/clw
)

install(EXPORT clwTarget
    NAMESPACE clw::
    DESTINATION lib/cmake/clw
    FILE clwTarget.cmake
)
