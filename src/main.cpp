 #include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <csignal>
#include <atomic>
#include "Server.hpp"
#include <spdlog/spdlog.h>

std::atomic<bool> running(true);

void handle_sigint(int) {
    spdlog::info("handle_sigint...");
    running = false;
}

void handle_sigterm(int) {
    spdlog::info("handle_sigterm...");
    running = false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        spdlog::error("Usage: {} <UpdateIntervalMs>", argv[0]);
        return 1;
    }
    int updateIntervalMs = std::atoi(argv[1]);
    if (updateIntervalMs <= 0) {
        spdlog::error("UpdateIntervalMs must be a positive integer.");
        return 1;
    }

    Server server;
    server.start(updateIntervalMs);

    // Set up signal handler for interrupt and shutdown
    std::signal(SIGINT, handle_sigint);

    // Set up signal handler for graceful shutdown
    std::signal(SIGTERM, handle_sigterm);

    // Main loop
    while (running) {
        // Debug: Fetch and print the latest data from the server
        const int latest_rpm = server.getLatestRpm();
        const int latest_temperature = server.getLatestTemperature();
        const int latest_oil_pressure = server.getLatestOilPressure();
        spdlog::info("Engine RPM:[{}], Temperature:[{}], Oil Pressure:[{}] psi", latest_rpm, latest_temperature, latest_oil_pressure);
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }

    spdlog::info("Shutting down...");
    server.stop();
    return 0;
}
