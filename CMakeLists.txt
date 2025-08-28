cmake_minimum_required(VERSION 3.10)
project(middlewaresw)

set(CMAKE_CXX_STANDARD 17)
find_package(Protobuf REQUIRED)

set(Protobuf_INCLUDE_DIR /usr/include)
set(Protobuf_LIBRARIES /usr/lib/x86_64-linux-gnu/libprotobuf.so)
include_directories(include ${Protobuf_INCLUDE_DIRS})

add_executable(middlewaresw src/main.cpp src/Receiver.cpp src/Engine.cpp include/engine_data.pb.cc)
target_link_libraries(middlewaresw ${Protobuf_LIBRARIES})

enable_testing()
add_subdirectory(tests)
