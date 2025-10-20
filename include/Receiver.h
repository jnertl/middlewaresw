#pragma once
#include <random>


class Receiver {
public:
    Receiver();
    int GetRpm();
    int GetTemperature();
    int GetOilPressure(); // psi, range 0-200
    int GetSpeed(); // km/h, range 0-500
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> rpmDist;
    std::uniform_int_distribution<int> tempDist{-50, 500};
    std::uniform_int_distribution<int> oilPressureDist{0, 200};
    std::uniform_int_distribution<int> speedDist{0, 500};
};
