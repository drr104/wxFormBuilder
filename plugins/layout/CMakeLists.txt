add_library(layout SHARED layout.cpp)

target_link_libraries(layout
    ticpp
    wxfb-plugin-interface
)

#add_custom_command(TARGET layout POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/liblayout${CMAKE_SHARED_LIBRARY_SUFFIX}
#    ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_LIB_DIR}plugins/layout/liblayout${CMAKE_SHARED_LIBRARY_SUFFIX}
#)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/liblayout${CMAKE_SHARED_LIBRARY_SUFFIX}
    DESTINATION ${INSTALL_LIB_DIR}/plugins/layout/
)
