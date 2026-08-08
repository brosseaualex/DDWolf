cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)

set(
        project_name
        DDWolf
)

file(
        GLOB_RECURSE
        source_files
        src/*
        win/*
)

project(${project_name})

find_package(SDL2 REQUIRED)
find_package(SDL2_MIXER REQUIRED)

include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
)

link_directories (
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_MIXER_LIBRARY_DIRS}
)

add_executable(
        ${project_name}
        ${source_files}
)

target_link_libraries(
        ${project_name}
        SDL2::SDL2main
        SDL2::SDL2
        SDL2_mixer::SDL2_mixer
        -static-libgcc
        -static-libstdc++
)