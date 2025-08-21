
add_executable(runUnitTests test_receiver.cpp test_engine.cpp ../src/Receiver.cpp ../src/Engine.cpp)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ../include)
target_link_libraries(runUnitTests ${GTEST_LIBRARIES} pthread)
add_test(NAME runUnitTests COMMAND runUnitTests)
