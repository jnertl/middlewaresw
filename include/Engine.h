#pragma once


#include "Receiver.h"
#include <chrono>

struct StoredValue {
    int value;
    std::chrono::system_clock::time_point timestamp;
};

class Engine {
public:
    virtual ~Engine() = default;
    virtual int getRpm() = 0;
    virtual int getTemperature()  = 0;
    virtual int getOilPressure() = 0;
    virtual StoredValue getStoredRpm() const = 0;
    virtual StoredValue getStoredTemperature() const = 0;
    virtual StoredValue getStoredOilPressure() const = 0;
};

class EngineImpl : public Engine {
public:
    EngineImpl();
    int getRpm() override;
    int getTemperature() override;
    int getOilPressure() override;
    StoredValue getStoredRpm() const override;
    StoredValue getStoredTemperature() const override;
    StoredValue getStoredOilPressure() const override;
private:
    Receiver receiver;
    StoredValue stored_rpm;
    StoredValue stored_temperature;
    StoredValue stored_oil_pressure;
};
