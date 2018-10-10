cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(smbuser VERSION 0.0.1 LANGUAGES C)

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(default_build_type "Release")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

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

install(TARGETS smbuseradd smbuserdel
        DESTINATION bin
        PERMISSIONS SETUID OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE)
