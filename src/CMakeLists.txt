cmake_minimum_required(VERSION 3.0.2)

list (APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

find_package(SDL2 2 COMPONENTS main image net ttf REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${SDL2_INCLUDE_DIR})
add_executable(pikachu main.cpp)
target_link_libraries(pikachu ${SDL2_LIBRARIES})
