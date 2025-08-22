
add_executable(runUnitTests test_receiver.cpp test_engine.cpp ../src/Receiver.cpp ../src/Engine.cpp ../include/engine_data.pb.cc)
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../include ${Protobuf_INCLUDE_DIRS})
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} pthread ${Protobuf_LIBRARIES})
add_test(NAME runUnitTests COMMAND runUnitTests)
