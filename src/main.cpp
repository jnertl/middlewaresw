#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "Engine.h"

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
    EngineImpl engine;
    while (true) {
        const int rpm = engine.getRpm();
        const int temperature = engine.getTemperature();
        std::cout << "Engine RPM:[" << rpm << "], Temperature:[" << temperature << "]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(updateIntervalMs));
    }
    return 0;
}
