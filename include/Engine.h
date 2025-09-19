#pragma once


#include "Receiver.h"


class Engine {
public:
    virtual ~Engine() = default;
    virtual int getRpm() = 0;
    virtual int getTemperature()  = 0;
    virtual int getOilPressure() = 0;
};

class EngineImpl : public Engine {
public:
    EngineImpl();
    int getRpm() override;
    int getTemperature() override;
    int getOilPressure() override;
private:
    Receiver receiver;
};
