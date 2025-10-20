#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

extern "C" {
    int socket(int, int, int) { return 42; }
    int bind(int, const struct sockaddr*, socklen_t) { return 0; }
    int listen(int, int) { return 0; }
    int accept(int, struct sockaddr*, socklen_t*) { return 43; }
    ssize_t read(int, void* buf, size_t count) {
        static int called = 0;
        if (called == 0) {
            // write a single byte to trigger the server's read>0 path
            if (count > 0) {
                static const char c = 'x';
                ((char*)buf)[0] = c;
                called = 1;
                return 1; // one byte read
            }
        }
        return 0; // subsequent calls signal EOF
    }
    ssize_t send(int, const void* , size_t count, int) { return (ssize_t)count; }
    int close(int) { return 0; }
    int setsockopt(int, int, int, const void*, socklen_t) { return 0; }
}
#include <gtest/gtest.h>
#include "Server.hpp"
#include <thread>
#include <chrono>

TEST(ServerTest, InitialValuesAreZero) {
    Server server;
    EXPECT_EQ(server.getLatestRpm(), 0);
    EXPECT_EQ(server.getLatestTemperature(), 0);
    EXPECT_EQ(server.getLatestOilPressure(), 0);
    EXPECT_EQ(server.getLatestSpeed(), 0);
}

TEST(ServerTest, DataUpdatesAfterStart) {
    Server server;
    server.start(200);
    int initial_rpm = server.getLatestRpm();
    int initial_temp = server.getLatestTemperature();
    int initial_oil = server.getLatestOilPressure();
    int initial_speed = server.getLatestSpeed();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    int updated_rpm = server.getLatestRpm();
    int updated_temp = server.getLatestTemperature();
    int updated_oil = server.getLatestOilPressure();
    int updated_speed = server.getLatestSpeed();
    server.stop();
    // Values should change after update thread runs
    EXPECT_NE(initial_rpm, updated_rpm);
    EXPECT_NE(initial_temp, updated_temp);
    EXPECT_NE(initial_oil, updated_oil);
    EXPECT_NE(initial_speed, updated_speed);
}

TEST(ServerTest, StartAndStopDoesNotThrow) {
    Server server;
    EXPECT_NO_THROW(server.start(100));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
}

// Note: Full socket/network tests would require integration or mocking, not pure unit tests.
// This test only checks basic construction and thread management.


