cmake_minimum_required(VERSION 3.28)
project(middlewaresw)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(runUnitTests test_main.cpp test_server.cpp test_receiver.cpp test_engine.cpp ../src/Server.cpp ../src/Receiver.cpp ../src/Engine.cpp ../include/engine_data.pb.cc)
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../include ${Protobuf_INCLUDE_DIRS})
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} pthread ${Protobuf_LIBRARIES})
add_test(NAME runUnitTests COMMAND runUnitTests)
