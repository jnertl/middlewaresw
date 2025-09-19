#include <gtest/gtest.h>
#include "Receiver.h"


TEST(ReceiverTest, GetTemperatureReturnsValueInRange) {
    Receiver receiver;
    for (int i = 0; i < 100; ++i) {
        int value = receiver.GetTemperature();
        EXPECT_GE(value, -50);
        EXPECT_LE(value, 500);
    }
}

TEST(ReceiverTest, GetReturnsValueInRange) {
    Receiver receiver;
    for (int i = 0; i < 8000; i+=100) {
        int value = receiver.GetRpm();
        EXPECT_GE(value, 0);
        EXPECT_LE(value, 8000);
    }
}

TEST(ReceiverTest, GetOilPressureReturnsValueInRange) {
    Receiver receiver;
    for (int i = 0; i < 100; ++i) {
        int value = receiver.GetOilPressure();
        EXPECT_GE(value, 0);
        EXPECT_LE(value, 200);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
