add_executable(clwinfo clwinfo.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo
    PROPERTIES
        # Can't use $<CONFIG> here.
        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_BUILD_TYPE})


add_executable(bandwidth bandwidth.cpp)
target_link_libraries(bandwidth PRIVATE clw::clw)
set_target_properties(bandwidth
    PROPERTIES
        # Can't use $<CONFIG> here.
        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_BUILD_TYPE})

install(TARGETS clwinfo bandwidth RUNTIME DESTINATION bin/$<CONFIG>)
