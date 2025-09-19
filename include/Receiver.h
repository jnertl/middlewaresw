#pragma once
#include <random>


class Receiver {
public:
    Receiver();
    int GetRpm();
    int GetTemperature();
    int GetOilPressure(); // psi, range 0-200
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> rpmDist;
    std::uniform_int_distribution<int> tempDist{-50, 500};
    std::uniform_int_distribution<int> oilPressureDist{0, 200};
};
