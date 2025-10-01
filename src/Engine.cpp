#include "Engine.h"

// All EngineImpl methods are defined inline in the header, but you can move them here if you want out-of-line definitions.

EngineImpl::EngineImpl() = default;

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
    return receiver.GetRpm();
}

int EngineImpl::getTemperature() {
    return receiver.GetTemperature();
}

int EngineImpl::getOilPressure() {
    return receiver.GetOilPressure();
}
