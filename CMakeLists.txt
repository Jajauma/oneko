cmake_minimum_required(VERSION 3.22)
project(TestProject LANGUAGES CXX)

option(UNICODE "Build with wide character APIs enabled" ON)
add_executable(TestProject WIN32 test.cc)

if (UNICODE)
    target_compile_definitions(TestProject PRIVATE UNICODE PRIVATE _UNICODE)
    # https://gitlab.kitware.com/cmake/cmake/-/issues/21202
    if (NOT CMAKE_GENERATOR MATCHES "Visual Studio")
        target_link_options(TestProject PRIVATE "/entry:wWinMainCRTStartup")
    endif()
endif()
