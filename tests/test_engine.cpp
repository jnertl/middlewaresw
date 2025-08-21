#include <gtest/gtest.h>
#include "Engine.h"

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
