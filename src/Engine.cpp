#include "Engine.h"

// All EngineImpl methods are defined inline in the header, but you can move them here if you want out-of-line definitions.

EngineImpl::EngineImpl() {
    // Initialize stored values with current time and default values
    auto now = std::chrono::system_clock::now();
    stored_rpm = {0, now};
    stored_temperature = {0, now};
    stored_oil_pressure = {0, now};
}

// Static variables for simulating a crash in getRpm()
static uint32_t untilCrashCounter = 0;
static const bool crashEnabled = false;

int EngineImpl::getRpm() {
    if (crashEnabled && ++untilCrashCounter >= 3) {
        untilCrashCounter = 0;
        // Simulate a crash
        int* p = nullptr;
        *p = 42; // Dereference null pointer to cause a crash
    }
    int value = receiver.GetRpm();
    stored_rpm = {value, std::chrono::system_clock::now()};
    return value;
}

int EngineImpl::getTemperature() {
    int value = receiver.GetTemperature();
    stored_temperature = {value, std::chrono::system_clock::now()};
    return value;
}

int EngineImpl::getOilPressure() {
    int value = receiver.GetOilPressure();
    stored_oil_pressure = {value, std::chrono::system_clock::now()};
    return value;
}

StoredValue EngineImpl::getStoredRpm() const {
    return stored_rpm;
}

StoredValue EngineImpl::getStoredTemperature() const {
    return stored_temperature;
}

StoredValue EngineImpl::getStoredOilPressure() const {
    return stored_oil_pressure;
}
