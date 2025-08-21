#include "Engine.h"

// All EngineImpl methods are defined inline in the header, but you can move them here if you want out-of-line definitions.

EngineImpl::EngineImpl() = default;

int EngineImpl::getRpm() {
    return receiver.GetRpm();
}

int EngineImpl::getTemperature() {
    return receiver.GetTemperature();
}
