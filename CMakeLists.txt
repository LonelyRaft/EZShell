
cmake_minimum_required(VERSION 3.20)

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES Windows)
    set(CMAKE_TOOLCHAIN_FILE mingw.cmake)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES Windows)
    set(CMAKE_TOOLCHAIN_FILE linuxgcc.cmake)
endif()

project(EZShell VERSION 1.0)

set(TARGET_NAME ezshell)
add_executable(${TARGET_NAME} "")
aux_source_directory(
    . TARGET_SRC_LIST)
target_sources(
    ${TARGET_NAME} PRIVATE
    ${TARGET_SRC_LIST}
)
