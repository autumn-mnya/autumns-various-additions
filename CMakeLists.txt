cmake_minimum_required(VERSION 3.16)

set(MOD_NAME autumns_various_additions)

project(${MOD_NAME} LANGUAGES C CXX)

option(MODCS_LUAJIT "Use LuaJIT for ModCS Lua API instead of Lua 5.4.6 (Default ON)" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_PATH ${CMAKE_SOURCE_DIR}/src)
set(COMMON_PATH ${CMAKE_SOURCE_DIR}/../common)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${SRC_PATH}/*.cpp
    ${SRC_PATH}/*.c
)

file(GLOB_RECURSE COMMON_FILES CONFIGURE_DEPENDS
    ${COMMON_PATH}/*.cpp
    ${COMMON_PATH}/*.c
)

add_library(${MOD_NAME} SHARED
    ${SRC_FILES}
    ${COMMON_FILES}
)

target_include_directories(${MOD_NAME} PRIVATE
    ${SRC_PATH}
    ${COMMON_PATH}
)

target_link_libraries(${MOD_NAME} PRIVATE shlwapi)

if (MODCS_LUAJIT)
    set(LUAJIT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/luajit)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    add_subdirectory("external/luajit-cmake")
    target_link_libraries(${MOD_NAME} PRIVATE luajit::lib luajit::header)
else()
    add_subdirectory(external/lua)
    target_link_libraries(${MOD_NAME} PRIVATE lua_static)
endif()

add_subdirectory("external/yaml-cpp")
target_link_libraries(${MOD_NAME} PRIVATE yaml-cpp::yaml-cpp)

if(MINGW)
    target_compile_options(${MOD_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
    target_link_options(${MOD_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
endif()

set_target_properties(${MOD_NAME} PROPERTIES
    OUTPUT_NAME "${MOD_NAME}"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/CaveStory/mods/${MOD_NAME}
)