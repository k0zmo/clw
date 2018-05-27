add_executable(clwinfo clwinfo.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo
    PROPERTIES
        INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib/$<CONFIG>)

install(TARGETS clwinfo EXPORT clwTargets 
    RUNTIME DESTINATION bin/$<CONFIG>
)
