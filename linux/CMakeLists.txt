cmake_minimum_required(VERSION 2.8.3)
project(oscilloscope_linux C CXX)

set(CMAKE_TOOLCHAIN_FILE src/cmake/arduino.cmake) 

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} liboscilloscope)