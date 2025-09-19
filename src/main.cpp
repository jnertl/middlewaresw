#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "Server.hpp"


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

    // Main loop
    while (true) {
        // Debug: Fetch and print the latest data from the server
        const int latest_rpm = server.getLatestRpm();
        const int latest_temperature = server.getLatestTemperature();
        const int latest_oil_pressure = server.getLatestOilPressure();
        std::cout << "Engine RPM:[" << latest_rpm << "], Temperature:[" << latest_temperature << "], Oil Pressure:[" << latest_oil_pressure << "] psi" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }

    server.stop();
    return 0;
}
