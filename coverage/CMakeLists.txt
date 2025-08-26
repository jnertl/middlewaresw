
cmake_minimum_required(VERSION 3.10)
project(test_coverage)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")

enable_testing()

find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(test_coverage
	../tests/test_receiver.cpp
	../tests/test_engine.cpp
	../src/Receiver.cpp
	../src/Engine.cpp
	../include/engine_data.pb.cc
)

target_include_directories(test_coverage PRIVATE ${GTEST_INCLUDE_DIRS} ../include ${Protobuf_INCLUDE_DIRS})
target_compile_options(test_coverage PRIVATE --coverage -O0 -g)
target_link_libraries(test_coverage PRIVATE ${GTEST_LIBRARIES} pthread ${Protobuf_LIBRARIES} --coverage)

add_test(NAME test_coverage COMMAND test_coverage)
