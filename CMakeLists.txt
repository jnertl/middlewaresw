cmake_minimum_required(VERSION 3.25)
project(middlewaresw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Protobuf REQUIRED)

set(Protobuf_INCLUDE_DIR /usr/include)
set(Protobuf_LIBRARIES /usr/lib/x86_64-linux-gnu/libprotobuf.so)
include_directories(include ${Protobuf_INCLUDE_DIRS})

add_subdirectory(external/spdlog)
include_directories(include external/spdlog/include)

add_executable(middlewaresw src/main.cpp src/Server.cpp src/Receiver.cpp src/Engine.cpp include/engine_data.pb.cc)
target_link_libraries(middlewaresw PRIVATE ${Protobuf_LIBRARIES} spdlog::spdlog_header_only)

enable_testing()
add_subdirectory(tests)
