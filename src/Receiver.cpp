#include "Receiver.h"
#include <random>


Receiver::Receiver() : rng(std::random_device{}()), rpmDist(0, 8000), tempDist(-50, 500), oilPressureDist(0, 200), speedDist(0, 500) {}

// Requirement: [REQ004] Engine Data Simulation
int Receiver::GetRpm() {
    return rpmDist(rng);
}

// Requirement: [REQ004] Engine Data Simulation
int Receiver::GetTemperature() {
    return tempDist(rng);
}

// Requirement: [REQ004] Engine Data Simulation
int Receiver::GetOilPressure() {
    return oilPressureDist(rng);
}

int Receiver::GetSpeed() {
    return speedDist(rng);
}
