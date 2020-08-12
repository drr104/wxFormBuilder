add_library(additional SHARED additional.cpp)

target_link_libraries(additional
    ticpp
    wxfb-plugin-interface
)

add_custom_command(TARGET additional POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libadditional.so
    ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/${INSTALL_LIB_DIR}/libadditional.so
)
