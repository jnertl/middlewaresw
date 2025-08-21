#include "Receiver.h"
#include <random>

Receiver::Receiver() : rng(std::random_device{}()), rpmDist(0, 8000), tempDist(-50, 500) {}

int Receiver::GetRpm() {
    return rpmDist(rng);
}

int Receiver::GetTemperature() {
    return tempDist(rng);
}
