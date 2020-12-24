include_directories(${CMAKE_SOURCE_DIR}/sdk/plugin_interface)

function(add_plugin plugin_name srcs)
    wxfb_add_library(${plugin_name} SHARED ${srcs})

    target_link_libraries(${plugin_name}
        PRIVATE
            ticpp
            wxfb-plugin-interface
    )

    target_compile_definitions(${plugin_name}
        PRIVATE
            BUILD_DLL
            TIXML_USE_TICPP
    )

    # Always use the unix style "lib" prefix
    set_target_properties(${plugin_name} 
        PROPERTIES
            PREFIX lib
    )

    if(WIN32)
        install(FILES
            $<TARGET_FILE:${plugin_name}>
            DESTINATION ${INSTALL_LIB_DIR}/plugins/${plugin_name}/
        )
    elseif(UNIX OR MAC)
        install(FILES
            $<TARGET_FILE:${plugin_name}>
            DESTINATION ${INSTALL_LIB_DIR}
        )
    endif()
endfunction()

add_subdirectory(additional)
add_subdirectory(common)
add_subdirectory(containers)
add_subdirectory(forms)
add_subdirectory(layout)
