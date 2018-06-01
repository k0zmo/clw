add_executable(clwinfo clwinfo.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo
    PROPERTIES
        # Can't use $<CONFIG> here.
        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_BUILD_TYPE})

install(TARGETS clwinfo RUNTIME DESTINATION bin/$<CONFIG>)
