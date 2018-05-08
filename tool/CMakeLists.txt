add_executable(clwinfo clwinfo.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo
    PROPERTIES
        DEBUG_POSTFIX d
        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CLW_LIBRARY_INSTALL_DIR})

install(TARGETS clwinfo EXPORT clwTargets 
    RUNTIME DESTINATION ${CLW_RUNTIME_INSTALL_DIR}
)
