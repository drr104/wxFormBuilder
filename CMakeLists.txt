cmake_minimum_required(VERSION 3.15)
project(wxFormBuilder 
    LANGUAGES CXX
    VERSION 3.9.0)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_CXX_STANDARD 17)
set(DEFAULT_BUILD_TYPE "Release")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
      STRING "Choose the type of build." FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(UNIX OR MSYS)
    set(wxWidgets_FIND_STYLE "unix")
endif()

find_package(wxWidgets 3.0 COMPONENTS gl aui adv html core net base xml stc richtext propgrid ribbon xrc REQUIRED)
include(${wxWidgets_USE_FILE})

set(CPACK_PACKAGE_VERSION_MAJOR PROJECT_VERSION_MAJOR)
set(CPACK_PACKAGE_VERSION_MINOR PROJECT_VERSION_MINOR)
set(CPACK_PACKAGE_VERSION_PATCH PROJECT_VERSION_PATCH)
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING)
set(CPACK_WIX_UPGRADE_GUID B5AAB525-34D5-4B12-B6F2-136C8ED890C9)

include(CPack)

set(INSTALL_ROOT_DIR "cmake_install")

if(WIN32)
    set(INSTALL_BIN_DIR ".")
    set(INSTALL_SHARE_DIR ".")
    set(INSTALL_LIB_DIR ".")
elseif(UNIX)
    set(INSTALL_BIN_DIR "bin")
    set(INSTALL_SHARE_DIR "share/wxformbuilder")
    set(INSTALL_LIB_DIR "lib/wxformbuilder")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/${INSTALL_ROOT_DIR})
set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ${INSTALL_BIN_DIR})

function(_add_target_common target)
    target_compile_definitions(${target} 
        PRIVATE 
            $<$<CONFIG:Debug>:__WXFB_DEBUG__>)

    target_compile_options(${target}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/MP>)
endfunction()

function(wxfb_add_executable target)
    add_executable(${target} ${ARGN})
    _add_target_common(${target})
endfunction()

function(wxfb_add_library target)
    add_library(${target} ${ARGN})
    _add_target_common(${target})
endfunction()

add_subdirectory(src)
add_subdirectory(subprojects/ticpp)
add_subdirectory(sdk/plugin_interface)
add_subdirectory(plugins)
