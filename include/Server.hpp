#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include "Engine.h"
#include "engine_data.pb.h"

class Server {

public: // Methods
    Server();
    void start(int updateIntervalMs);
    void stop();
    int getLatestRpm();
    int getLatestTemperature();
    int getLatestOilPressure();
    int getLatestSpeed();

private: // Methods
    void run();
    void updateDataLoop();

private: // Data members
    EngineImpl engine;
    std::mutex data_mutex;
    int updateIntervalMs;
    int latest_rpm;
    int latest_temperature;
    int latest_oil_pressure;
    int latest_speed;
    std::atomic<bool> running;
    std::thread server_thread;
    std::thread data_thread;
};
