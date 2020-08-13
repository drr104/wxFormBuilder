set(wxFB_SRCS
    maingui.cpp
    md5/md5.cc
    dbg_stack_trace/stack.cpp
    rad/about.cpp
    rad/appdata.cpp
    rad/bitmaps.cpp
    rad/cmdproc.cpp
    rad/customkeys.cpp
    rad/dialogfindcomponent.cpp
    rad/genericpanel.cpp
    rad/mainframe.cpp
    rad/menueditor.cpp
    rad/palette.cpp
    rad/title.cpp
    rad/wxfbevent.cpp
    rad/wxfbmanager.cpp
    rad/codeeditor/codeeditor.cpp
    rad/cpppanel/cpppanel.cpp
    rad/dataobject/dataobject.cpp
    rad/designer/innerframe.cpp
    rad/designer/menubar.cpp
    rad/designer/resizablepanel.cpp
    rad/designer/visualeditor.cpp
    rad/designer/visualobj.cpp
    rad/geninheritclass/geninhertclass.cpp
    rad/geninheritclass/geninhertclass_gui.cpp
    rad/inspector/objinspect.cpp
    rad/inspector/wxfbadvprops.cpp
    rad/luapanel/luapanel.cpp
    rad/objecttree/objecttree.cpp
    rad/phppanel/phppanel.cpp
    rad/pythonpanel/pythonpanel.cpp
    rad/xrcpanel/xrcpanel.cpp
    rad/xrcpreview/xrcpreview.cpp
    utils/filetocarray.cpp
    utils/annoyingdialog.cpp
    utils/stringutils.cpp
    utils/wxfbipc.cpp
    utils/typeconv.cpp
    codegen/phpcg.cpp
    codegen/codeparser.cpp
    codegen/xrccg.cpp
    codegen/luacg.cpp
    codegen/cppcg.cpp
    codegen/codegen.cpp
    codegen/pythoncg.cpp
    codegen/codewriter.cpp
    model/xrcfilter.cpp
    model/objectbase.cpp
    model/types.cpp
    model/database.cpp
)

if(WIN32)
    list(APPEND wxFB_SRCS wxFormBuilder.rc)
elseif(APPLE)
    set(wxFB_SRCS ${wxFB_SRCS} wxFormBuilder.icns)
    set(MACOSX_BUNDLE_ICON_FILE wxFormBuilder.icns)
    set_source_files_properties(wxFormBuilder.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)

    file(GLOB XMLS ../xml/*)
    list(APPEND wxFB_SRCS ${XMLS})
    set_source_files_properties(${XMLS} PROPERTIES MACOSX_PACKAGE_LOCATION "SharedSupport/xml")
    source_group("Resources\\xml" FILES ${XMLS})

    file(GLOB RES LIST_DIRECTORIES false ../resources/*)
    list(APPEND wxFB_SRCS ${RES})
    set_source_files_properties(${RES} PROPERTIES MACOSX_PACKAGE_LOCATION "SharedSupport/resources")
    source_group("Resources" FILES ${RES})

    file(GLOB ICONS LIST_DIRECTORIES false ../resources/icons/*)
    list(APPEND wxFB_SRCS ${ICONS})
    set_source_files_properties(${ICONS} PROPERTIES MACOSX_PACKAGE_LOCATION "SharedSupport/resources/icons")
    source_group("Resources\\Icons" FILES ${ICONS})
endif()

add_executable(wxFormBuilder WIN32 MACOSX_BUNDLE
    ${wxFB_SRCS}
)

#if(APPLE)
#    set_target_properties(wxFormBuilder PROPERTIES
#        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
#        MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxformbuilder.rad"
#        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CPACK_PACKAGE_VERSION}"
#    )
#else()
#    add_custom_command(TARGET wxFormBuilder POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/output/resources
#        ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_SHARE_DIR}resources
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/output/xml
#        ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_SHARE_DIR}xml
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/output/plugins
#        ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_SHARE_DIR}plugins
#        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/wxFormBuilder${CMAKE_EXECUTABLE_SUFFIX}
#        ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR}${INSTALL_BIN_DIR}wxFormBuilder${CMAKE_EXECUTABLE_SUFFIX}
#    )
#endif()

target_include_directories(wxFormBuilder
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(wxFormBuilder
    ${wxWidgets_LIBRARIES}
    ticpp
    wxfb-plugin-interface
)

if(MINGW)
    find_path(BINUTILS_BASE_INCLUDE_DIR binutils/bfd.h)
    target_include_directories(wxFormBuilder
        PRIVATE ${BINUTILS_BASE_INCLUDE_DIR}/binutils
    )
endif()

if(WIN32)
    if(MSVC)
        target_link_libraries(wxFormBuilder Dbghelp)
    else()
        find_library(BFD_LIB NAMES binutils/libbfd.a libbfd.a)
        find_library(IBERTY_LIB NAMES binutils/libiberty.a libiberty.a)
        find_package(ZLIB REQUIRED)
        find_package(Intl REQUIRED)

        target_link_libraries(wxFormBuilder
            ${BFD_LIB}
            ${IBERTY_LIB}
            ${Intl_LIBRARIES}
            ${ZLIB_LIBRARIES}
            psapi
            imagehlp)
    endif()
endif()

if(MSYS)
    find_path(MSYS_PATH include/binutils/bfd.h)
    set(MINGW_BIN_DIR "${MSYS_PATH}/bin")
    set(MINGW_MODULES
        libexpat
        libgcc
        libiconv
        libintl
        libjpeg
        liblzma
        libpng
        libstdc++
        libtiff
        libwinpthread
        libzstd
        zlib
        wx
        )
    foreach(mod IN LISTS MINGW_MODULES)
        file(GLOB mods "${MINGW_BIN_DIR}/${mod}*.dll")
        list(APPEND MINGW_FILES ${mods})
    endforeach()
    install(FILES ${MINGW_FILES} DESTINATION .)
#elseif(WIN32)
#    file(GLOB WXLIBS "${wxWidgets_LIB_DIR}/wx*.dll")
#    foreach(lib IN LISTS WXLIBS)
#        if(${lib} MATCHES "ud_*")
#            list(APPEND WXLIBS_DEBUG ${lib})
#        else()
#            list(APPEND WXLIBS_RELEASE ${lib})
#        endif()
#    endforeach()
    
#    install(FILES ${WXLIBS_DEBUG} DESTINATION . CONFIGURATIONS Debug)
#    install(FILES ${WXLIBS_RELEASE} DESTINATION . CONFIGURATIONS Release)
endif()

install(TARGETS wxFormBuilder
    BUNDLE DESTINATION .
    RUNTIME DESTINATION .
)

if(APPLE)
#    set(fixup_exe "\${CMAKE_INSTALL_PREFIX}/wxFormBuilder.app")
#    set(FB_PLUGINS common additional containers forms layout)

#    foreach(plugin IN LISTS FB_PLUGINS)
#        set(fixup_libs ${fixup_libs} ${fixup_exe}/Contents/PlugIns/${CMAKE_SHARED_LIBRARY_PREFIX}${plugin}${CMAKE_SHARED_LIBRARY_SUFFIX})
#    endforeach()

#    install(CODE "
#    include(\"BundleUtilities\")
#    fixup_bundle(\"${fixup_exe}\" \"${fixup_libs}\" \"\")
#    " ${COMPONENT})
else()
    install(DIRECTORY
        ${CMAKE_SOURCE_DIR}/output/xml
        ${CMAKE_SOURCE_DIR}/output/resources
        ${CMAKE_SOURCE_DIR}/output/plugins
        DESTINATION .
        )
endif()
