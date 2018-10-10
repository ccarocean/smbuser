cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(smbuser VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_compile_options("$<$<CONFIG:DEBUG>:-Werror>")
    add_compile_options(-Wdocumentation -Wno-documentation-unknown-command)
    add_compile_options(-Weverything)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options("$<$<CONFIG:DEBUG>:-Werror>")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-psabi)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
endif ()


add_executable(smbuseradd
        src/smbuseradd.c
        src/common.c
        src/pam.c
        src/smb.c
        )
target_link_libraries(smbuseradd pam)

add_executable(smbuserdel
        src/smbuserdel.c
        src/common.c
        src/pam.c
        src/smb.c
        )
target_link_libraries(smbuserdel pam)
