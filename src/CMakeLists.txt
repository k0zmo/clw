add_library(clw
    ${clw_SOURCE_DIR}/include/clw/clw.h
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

include(GenerateExportHeader)
generate_export_header(clw)
target_sources(clw PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/clw_export.h)

add_library(clw::clw ALIAS clw)
target_link_libraries(clw PUBLIC OpenCL::OpenCL)
target_include_directories(clw 
    PUBLIC 
        $<BUILD_INTERFACE:${clw_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
target_compile_definitions(clw
    PRIVATE 
        HAVE_OPENCL_1_1
        $<$<BOOL:${CLW_ENABLE_OPENCL_1_2}>:HAVE_OPENCL_1_2>
        $<$<NOT:$<BOOL:${CLW_ENABLE_OPENCL_1_2}>>:CL_USE_DEPRECATED_OPENCL_1_1_APIS>

        # Some OpenCL implementations use 2_0 for functions deprecated in 2.0 (Windows, AMD APP SDK)
        # but some other implementaions use 1_2 for functions deprecated in 2.0 (Linux) ;/
        # In former the 1_2 is not even defined
        CL_USE_DEPRECATED_OPENCL_1_2_APIS
        CL_USE_DEPRECATED_OPENCL_2_0_APIS
)

if(BUILD_SHARED_LIBS)
    if(MSVC)
        # Disable warning: 'X' needs to have dll-interface to be used by clients of class
        target_compile_options(clw PUBLIC /wd4251)
    endif()
endif()

target_compile_features(clw PUBLIC cxx_std_11)

install(TARGETS clw EXPORT clwTarget
    INCLUDES DESTINATION include
    RUNTIME DESTINATION bin/$<CONFIG>
    LIBRARY DESTINATION lib/$<CONFIG>
    ARCHIVE DESTINATION lib/$<CONFIG>
)
install(DIRECTORY ${clw_SOURCE_DIR}/include/clw
    DESTINATION include
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/clw_export.h
    DESTINATION include/clw
)
