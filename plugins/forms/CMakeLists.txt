add_library(forms SHARED forms.cpp)
target_link_libraries(forms
    ticpp
    wxfb-plugin-interface
)
add_custom_command(TARGET forms POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libforms.so
        ${CMAKE_SOURCE_DIR}/cmake_install/${INSTALL_LIB_DIR}/libforms.so
    )
