 #include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <csignal>
#include <atomic>
#include "Server.hpp"


std::atomic<bool> running(true);

void handle_sigint(int) {
    std::cout << "handle_sigint..." << std::endl;
    running = false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <UpdateIntervalMs>" << std::endl;
        return 1;
    }
    int updateIntervalMs = std::atoi(argv[1]);
    if (updateIntervalMs <= 0) {
        std::cerr << "UpdateIntervalMs must be a positive integer." << std::endl;
        return 1;
    }

    Server server;
    server.start(updateIntervalMs);

    // Set up signal handler for graceful shutdown
    std::signal(SIGINT, handle_sigint);

    // Main loop
    while (running) {
        // Debug: Fetch and print the latest data from the server
        const int latest_rpm = server.getLatestRpm();
        const int latest_temperature = server.getLatestTemperature();
        const int latest_oil_pressure = server.getLatestOilPressure();
        std::cout << "Engine RPM:[" << latest_rpm << "], Temperature:[" << latest_temperature << "], Oil Pressure:[" << latest_oil_pressure << "] psi" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }

    std::cout << "Shutting down..." << std::endl;
    server.stop();
    return 0;
}
