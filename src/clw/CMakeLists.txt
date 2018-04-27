set(CLW_PUBLIC_HEADERS
    clw.h
    config.h
    Buffer.h
    CommandQueue.h
    Context.h
    Device.h
    DeviceFilter.h
    EnumFlags.h
    Event.h
    Grid.h
    Image.h
    Kernel.h
    KernelTypesTraits.h
    MemoryObject.h
    Platform.h
    Prerequisites.h
    Program.h
    Sampler.h
    TypeTraits.h
)

add_library(clw ${CLW_PUBLIC_HEADERS})

target_sources(clw PRIVATE 
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
    INTERFACE 
        $<BUILD_INTERFACE:${clw_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
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

if(MSVC)
    target_compile_options(clw 
        PUBLIC
            /std:c++latest
            $<$<VERSION_GREATER_EQUAL:${MSVC_VERSION},1910>:/permissive->
    )
    target_compile_options(clw 
        PRIVATE
            /W4
        PUBLIC
            $<$<BOOL:${BUILD_SHARED_LIBS}>:/wd4251>
    )
elseif(CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    target_compile_features(clw PUBLIC cxx_std_14)
    target_compile_options(clw 
        PRIVATE 
            -Wall
            -pedantic
            -Wextra
    )
endif()

install(TARGETS clw
    EXPORT clwTarget
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(FILES ${CLW_PUBLIC_HEADERS} 
    DESTINATION include/clw
)

configure_file("${clw_SOURCE_DIR}/cmake/clwConfig.cmake"
    ${CMAKE_CURRENT_BINARY_DIR}/clwConfig.cmake
    COPYONLY
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/clwConfig.cmake"
    DESTINATION lib/cmake/clw
)
install(EXPORT clwTarget
    NAMESPACE clw::
    DESTINATION lib/cmake/clw
    FILE clwTarget.cmake
)

# Export from the build tree
export(EXPORT clwTarget 
    NAMESPACE clw::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/clwTarget.cmake
)
export(PACKAGE clw)
