cmake_minimum_required(VERSION 3.18)

project(ChaosCats)

set(SFML_DIR "C:/CppLibs/SFML/SFML-2.6.1_static")
set(ASIO_DIR "C:/CppLibs/Asio/asio-1.30.2")

set(SFML_LIB ${SFML_DIR}/lib)
set(SFML_INCLUDE ${SFML_DIR}/include)

set(ASIO_INCLUDE ${ASIO_DIR}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(ChaosCats ${SOURCES})

set_property(TARGET ChaosCats PROPERTY CXX_STANDARD 17)
set_property(TARGET ChaosCats PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_options(ChaosCats PRIVATE -O3)

include_directories(${SFML_INCLUDE} ${ASIO_INCLUDE})

target_precompile_headers(ChaosCats
    PRIVATE
    "src/chaoscats/net/framework/net.hpp"
)

if (CMAKE_SYSTEM MATCHES Windows)
    target_link_libraries(ChaosCats
        -static-libgcc
        -static-libstdc++
        ${SFML_LIB}/libsfml-window-s.a
        ${SFML_LIB}/libsfml-graphics-s.a
        ${SFML_LIB}/libsfml-system-s.a
        opengl32
        winmm
        gdi32
        ${SFML_LIB}/libfreetype.a
        wsock32
        ws2_32
)
endif()

target_compile_definitions(ChaosCats PRIVATE SFML_STATIC)