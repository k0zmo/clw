cmake_minimum_required(VERSION 3.9)
project(clw LANGUAGES CXX VERSION 0.1)

find_package(OpenCL REQUIRED)

option(CLW_ENABLE_OPENCL_1_2 "Enable OpenCL 1.2 features" ON)

if(UNIX)
    include(GNUInstallDirs)
    set(clw_RUNTIME_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
    set(clw_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
    set(clw_ARCHIVE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
elseif(WIN32)
    set(clw_RUNTIME_INSTALL_DIR bin)
    set(clw_LIBRARY_INSTALL_DIR bin)
    set(clw_ARCHIVE_INSTALL_DIR lib)
endif()
set(clw_CMAKE_CONFIG_INSTALL_DIR lib/cmake)
set(clw_INCLUDE_INSTALL_DIR include)

add_subdirectory(src)
add_subdirectory(tool)

# Installation stuff
set(clw_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/clwConfig.cmake)
set(clw_CONFIG_VERSION_FILE ${CMAKE_CURRENT_BINARY_DIR}/clwConfigVersion.cmake)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(${clw_CONFIG_VERSION_FILE}
                                 VERSION ${clw_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

file(WRITE ${clw_CONFIG_FILE}
"include(CMakeFindDependencyMacro)
find_dependency(OpenCL)
include(\"\${CMAKE_CURRENT_LIST_DIR}/clwTarget.cmake\")
")

install(FILES ${clw_CONFIG_FILE} ${clw_CONFIG_VERSION_FILE}
    DESTINATION ${clw_CMAKE_CONFIG_INSTALL_DIR}/clw
)

install(EXPORT clwTarget
    NAMESPACE clw::
    DESTINATION ${clw_CMAKE_CONFIG_INSTALL_DIR}/clw
    FILE clwTarget.cmake
)
