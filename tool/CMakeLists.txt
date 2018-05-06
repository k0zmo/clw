add_executable(clwinfo clwinfo.cpp)
target_link_libraries(clwinfo PRIVATE clw::clw)
set_target_properties(clwinfo PROPERTIES DEBUG_POSTFIX d)
target_compile_features(clwinfo PUBLIC cxx_std_11)

install(TARGETS clwinfo EXPORT clwTargets RUNTIME DESTINATION ${clw_RUNTIME_INSTALL_DIR})
