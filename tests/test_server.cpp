#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Mocks for system calls used by Server. Behavior can be toggled via the globals below.
static int mock_socket_ret = 42;
static int mock_fcntl_fail = 0;
static int mock_setsockopt_fail = 0;
static int mock_bind_fail = 0;
static int mock_listen_fail = 0;

extern "C" {
    int socket(int, int, int) { return mock_socket_ret; }
    int bind(int, const struct sockaddr*, socklen_t) { return mock_bind_fail ? -1 : 0; }
    int listen(int, int) { return mock_listen_fail ? -1 : 0; }
    int accept(int, struct sockaddr*, socklen_t*) { return 43; }
    ssize_t read(int, void* buf, size_t count) {
        static int called = 0;
        if (called == 0) {
            if (count > 0) {
                static const char c = 'x';
                ((char*)buf)[0] = c;
                called = 1;
                return 1; // one byte read
            }
        }
        return 0; // subsequent calls signal EOF
    }
    ssize_t send(int, const void*, size_t count, int) { return (ssize_t)count; }
    int close(int) { return 0; }
    int setsockopt(int, int, int, const void*, socklen_t) { return mock_setsockopt_fail ? -1 : 0; }
    int fcntl(int fd, int cmd, int arg) { (void)fd; (void)arg; if (mock_fcntl_fail) return -1; return 0; }
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

TEST(ServerErrorTest, SocketFailureHandled) {
    mock_socket_ret = -1;
    Server server;
    // start should not throw; the server thread should exit gracefully
    EXPECT_NO_THROW(server.start(50));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
    mock_socket_ret = 42;
}

TEST(ServerErrorTest, FcntlFailureHandled) {
    mock_fcntl_fail = 1;
    Server server;
    EXPECT_NO_THROW(server.start(50));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
    mock_fcntl_fail = 0;
}

TEST(ServerErrorTest, SetsockoptFailureHandled) {
    mock_setsockopt_fail = 1;
    Server server;
    EXPECT_NO_THROW(server.start(50));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
    mock_setsockopt_fail = 0;
}

TEST(ServerErrorTest, BindFailureHandled) {
    mock_bind_fail = 1;
    Server server;
    EXPECT_NO_THROW(server.start(50));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
    mock_bind_fail = 0;
}

TEST(ServerErrorTest, ListenFailureHandled) {
    mock_listen_fail = 1;
    Server server;
    EXPECT_NO_THROW(server.start(50));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NO_THROW(server.stop());
    mock_listen_fail = 0;
}

// Note: Full socket/network tests would require integration or mocking, not pure unit tests.
// This test only checks basic construction and thread management.


