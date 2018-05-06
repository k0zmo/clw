add_library(clw
    ${clw_SOURCE_DIR}/include/clw/clw.h
    ${clw_SOURCE_DIR}/include/clw/config.h
    ${clw_SOURCE_DIR}/include/clw/Buffer.h
    ${clw_SOURCE_DIR}/include/clw/CommandQueue.h
    ${clw_SOURCE_DIR}/include/clw/Context.h
    ${clw_SOURCE_DIR}/include/clw/Device.h
    ${clw_SOURCE_DIR}/include/clw/DeviceFilter.h
    ${clw_SOURCE_DIR}/include/clw/EnumFlags.h
    ${clw_SOURCE_DIR}/include/clw/Event.h
    ${clw_SOURCE_DIR}/include/clw/Grid.h
    ${clw_SOURCE_DIR}/include/clw/Image.h
    ${clw_SOURCE_DIR}/include/clw/Kernel.h
    ${clw_SOURCE_DIR}/include/clw/KernelTypesTraits.h
    ${clw_SOURCE_DIR}/include/clw/MemoryObject.h
    ${clw_SOURCE_DIR}/include/clw/Platform.h
    ${clw_SOURCE_DIR}/include/clw/Prerequisites.h
    ${clw_SOURCE_DIR}/include/clw/Program.h
    ${clw_SOURCE_DIR}/include/clw/Sampler.h
    ${clw_SOURCE_DIR}/include/clw/TypeTraits.h
    Buffer.cpp
    CommandQueue.cpp
    Context.cpp
    Device.cpp
    Event.cpp
    Grid.cpp
    Image.cpp
    Kernel.cpp
    MemoryObject.cpp
    Platform.cpp
    Program.cpp
    Sampler.cpp
    details.cpp
    details.h
)

add_library(clw::clw ALIAS clw)
target_link_libraries(clw PUBLIC OpenCL::OpenCL)
target_include_directories(clw 
    PUBLIC 
        $<BUILD_INTERFACE:${clw_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${clw_INCLUDE_INSTALL_DIR}>
)
target_compile_definitions(clw
    PRIVATE 
        HAVE_OPENCL_1_1
        $<$<BOOL:{CLW_ENABLE_OPENCL_1_2}>:HAVE_OPENCL_1_2>
)
set_target_properties(clw PROPERTIES DEBUG_POSTFIX "d")

if(BUILD_SHARED_LIBS)
    target_compile_definitions(clw PRIVATE CLW_BUILD_SHARED)
else()
    target_compile_definitions(clw PUBLIC CLW_STATIC_LIB)
endif()

target_compile_features(clw PUBLIC cxx_std_11)

if(MSVC)
    target_compile_options(clw 
        PRIVATE
            /W4
        PUBLIC
            $<$<BOOL:${BUILD_SHARED_LIBS}>:/wd4251>
    )
elseif(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    target_compile_options(clw 
        PRIVATE 
            -Wall
            -pedantic
            -Wextra
    )
endif()

install(TARGETS clw EXPORT clwTarget
    RUNTIME DESTINATION ${clw_RUNTIME_INSTALL_DIR}
    LIBRARY DESTINATION ${clw_LIBRARY_INSTALL_DIR}
    ARCHIVE DESTINATION ${clw_ARCHIVE_INSTALL_DIR}
)
install(DIRECTORY ${clw_SOURCE_DIR}/include/clw
    DESTINATION ${clw_INCLUDE_INSTALL_DIR}
)
