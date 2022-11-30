
cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE mingw.cmake)

project(Main VERSION 1.0)

add_executable(${PROJECT_NAME} "")

target_sources(
    ${PROJECT_NAME} PRIVATE
    main.c
    ezshell.c
    cmds.c
    ezshell.h
)
