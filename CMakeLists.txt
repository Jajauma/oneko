cmake_minimum_required(VERSION 3.22)
project(TestProject LANGUAGES CXX)

option(UNICODE "Build with wide character APIs enabled" ON)
option(ENABLE_WARNINGS "Build with compiler warnings enabled" ON)
add_executable(TestProject WIN32 test.cc)

if (UNICODE)
    target_compile_definitions(TestProject PRIVATE UNICODE PRIVATE _UNICODE)
endif()

# allow mingw and winelib
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(TestProject PRIVATE "-mwindows")
    if (UNICODE)
        target_link_options(TestProject PRIVATE "-municode")
    endif()
endif()

# allow ninja with visual studio
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"
        AND NOT CMAKE_GENERATOR MATCHES "Visual Studio"
        AND UNICODE)
    target_link_options(TestProject PRIVATE "/entry:wWinMainCRTStartup")
endif()

if (ENABLE_WARNINGS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(TestProject PRIVATE "-Wall -Wextra")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(TestProject PRIVATE "/W3")
    endif()
endif()
