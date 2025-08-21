cmake_minimum_required(VERSION 3.10)
project(middlewaresw)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(middlewaresw src/main.cpp src/Receiver.cpp src/Engine.cpp)

enable_testing()
add_subdirectory(tests)
