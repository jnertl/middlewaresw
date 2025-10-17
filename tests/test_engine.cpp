#include <gtest/gtest.h>
#include "Engine.h"
#include <thread>
#include <chrono>


TEST(EngineTest, GetRpmReturnsValueInRange) {
    EngineImpl engine;
    int value = engine.getRpm();
    EXPECT_GE(value, 0);
    EXPECT_LE(value, 8000);
}

TEST(EngineTest, GetTemperatureReturnsValueInRange) {
    EngineImpl engine;
    int value = engine.getTemperature();
    EXPECT_GE(value, -50);
    EXPECT_LE(value, 500);
}

TEST(EngineTest, GetOilPressureReturnsValueInRange) {
    EngineImpl engine;
    int value = engine.getOilPressure();
    EXPECT_GE(value, 0);
    EXPECT_LE(value, 200);
}

TEST(EngineTest, StoredRpmHasValue) {
    EngineImpl engine;
    int value = engine.getRpm();
    StoredValue stored = engine.getStoredRpm();
    EXPECT_EQ(stored.value, value);
    EXPECT_GE(stored.value, 0);
    EXPECT_LE(stored.value, 8000);
}

TEST(EngineTest, StoredTemperatureHasValue) {
    EngineImpl engine;
    int value = engine.getTemperature();
    StoredValue stored = engine.getStoredTemperature();
    EXPECT_EQ(stored.value, value);
    EXPECT_GE(stored.value, -50);
    EXPECT_LE(stored.value, 500);
}

TEST(EngineTest, StoredOilPressureHasValue) {
    EngineImpl engine;
    int value = engine.getOilPressure();
    StoredValue stored = engine.getStoredOilPressure();
    EXPECT_EQ(stored.value, value);
    EXPECT_GE(stored.value, 0);
    EXPECT_LE(stored.value, 200);
}

TEST(EngineTest, StoredRpmHasTimestamp) {
    EngineImpl engine;
    auto before = std::chrono::system_clock::now();
    engine.getRpm();
    auto after = std::chrono::system_clock::now();
    
    StoredValue stored = engine.getStoredRpm();
    EXPECT_GE(stored.timestamp, before);
    EXPECT_LE(stored.timestamp, after);
}

TEST(EngineTest, StoredTemperatureHasTimestamp) {
    EngineImpl engine;
    auto before = std::chrono::system_clock::now();
    engine.getTemperature();
    auto after = std::chrono::system_clock::now();
    
    StoredValue stored = engine.getStoredTemperature();
    EXPECT_GE(stored.timestamp, before);
    EXPECT_LE(stored.timestamp, after);
}

TEST(EngineTest, StoredOilPressureHasTimestamp) {
    EngineImpl engine;
    auto before = std::chrono::system_clock::now();
    engine.getOilPressure();
    auto after = std::chrono::system_clock::now();
    
    StoredValue stored = engine.getStoredOilPressure();
    EXPECT_GE(stored.timestamp, before);
    EXPECT_LE(stored.timestamp, after);
}

TEST(EngineTest, TimestampUpdatesOnSubsequentCalls) {
    EngineImpl engine;
    engine.getRpm();
    StoredValue first = engine.getStoredRpm();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    engine.getRpm();
    StoredValue second = engine.getStoredRpm();
    
    EXPECT_GT(second.timestamp, first.timestamp);
}
