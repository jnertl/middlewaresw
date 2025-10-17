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
};

class EngineImpl : public Engine {
public:
    EngineImpl();
    EngineImpl(const std::string& db_path);
    ~EngineImpl();
    int getRpm() override;
    int getTemperature() override;
    int getOilPressure() override;
private:
    Receiver receiver;
    sqlite3* db;
    void initDatabase(const std::string& db_path);
    void storeValue(int rpm, int temperature, int oil_pressure);
};
