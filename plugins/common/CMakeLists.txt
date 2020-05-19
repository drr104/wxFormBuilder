add_library(common SHARED common.cpp)
target_link_libraries(common
    ticpp
    wxfb-plugin-interface
)
add_custom_command(TARGET common POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libcommon.so
        ${CMAKE_SOURCE_DIR}/cmake_install/${INSTALL_LIB_DIR}/libcommon.so
    )
