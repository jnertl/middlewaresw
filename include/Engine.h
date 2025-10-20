#pragma once


#include "Receiver.h"
#include <sqlite3.h>
#include <string>


class Engine {
public:
    virtual ~Engine() = default;
    virtual int getRpm() = 0;
    virtual int getTemperature()  = 0;
    virtual int getOilPressure() = 0;
    virtual int getSpeed() = 0;
    virtual void storeCurrentValues(int rpm, int temperature, int oil_pressure, int speed) = 0;
};

class EngineImpl : public Engine {
public:
    EngineImpl();
    EngineImpl(const std::string& db_path);
    ~EngineImpl();
    int getRpm() override;
    int getTemperature() override;
    int getOilPressure() override;
    int getSpeed() override;
    void storeCurrentValues(int rpm, int temperature, int oil_pressure, int speed) override;
private:
    Receiver receiver;
    sqlite3* db;
    void initDatabase(const std::string& db_path);
};
