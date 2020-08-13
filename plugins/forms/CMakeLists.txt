add_library(forms SHARED forms.cpp)

target_link_libraries(forms
    ticpp
    wxfb-plugin-interface
)

add_custom_command(TARGET forms POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libforms${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_LIB_DIR}plugins/forms/libforms${CMAKE_SHARED_LIBRARY_SUFFIX}
)
