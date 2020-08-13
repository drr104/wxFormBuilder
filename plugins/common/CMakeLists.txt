add_library(common SHARED common.cpp)

target_link_libraries(common
    ticpp
    wxfb-plugin-interface
)

add_custom_command(TARGET common POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/libcommon${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_LIB_DIR}plugins/common/libcommon${CMAKE_SHARED_LIBRARY_SUFFIX}
)
