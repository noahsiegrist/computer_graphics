cmake_minimum_required(VERSION 3.26)
project(01_clion)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED SDL2)

add_executable(01_clion src/main.cpp
        src/CornellBox.cpp
        src/CornellBox.h
        src/Sphere.cpp
        src/Sphere.h
        src/Color.h
        src/Color.cpp)
target_link_libraries(01_clion PRIVATE SDL2::SDL2)