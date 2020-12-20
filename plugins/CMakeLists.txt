include_directories(${CMAKE_SOURCE_DIR}/sdk/plugin_interface)

function(add_plugin plugin_name srcs)
    wxfb_add_library(${plugin_name} SHARED ${srcs})

    target_link_libraries(${plugin_name}
        PRIVATE
            ticpp
            wxfb-plugin-interface
    )

    if(MSYS)
        install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${plugin_name}${CMAKE_SHARED_LIBRARY_SUFFIX}
            DESTINATION ${INSTALL_LIB_DIR}/plugins/${plugin_name}/
        )
    elseif(UNIX)
        install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}${plugin_name}${CMAKE_SHARED_LIBRARY_SUFFIX}
            DESTINATION ${INSTALL_LIB_DIR}
        )
    endif()
endfunction()

add_subdirectory(additional)
add_subdirectory(common)
add_subdirectory(containers)
add_subdirectory(forms)
add_subdirectory(layout)
