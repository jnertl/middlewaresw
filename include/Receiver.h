#pragma once
#include <random>

class Receiver {
public:
    Receiver();
    int GetRpm();
    int GetTemperature();
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> rpmDist;
    std::uniform_int_distribution<int> tempDist{-50, 500};
};
