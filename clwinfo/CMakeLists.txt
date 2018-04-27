add_executable(clwinfo main.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo PROPERTIES DEBUG_POSTFIX d)

install(TARGETS clwinfo DESTINATION bin)
