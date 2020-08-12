add_library(containers SHARED containers.cpp)

target_link_libraries(containers
    ticpp
    wxfb-plugin-interface
)

add_custom_command(TARGET containers POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libcontainers.so
    ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}/${INSTALL_LIB_DIR}/libcontainers.so
)

