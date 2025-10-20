#include <gtest/gtest.h>
#include "Engine.h"
#include <sqlite3.h>
#include <filesystem>
#include <thread>
#include <chrono>


TEST(EngineTest, GetRpmReturnsValueInRange) {
    EngineImpl engine("/tmp/test_engine_rpm.db");
    int value = engine.getRpm();
    EXPECT_GE(value, 0);
    EXPECT_LE(value, 8000);
    std::filesystem::remove("/tmp/test_engine_rpm.db");
}

TEST(EngineTest, GetTemperatureReturnsValueInRange) {
    EngineImpl engine("/tmp/test_engine_temp.db");
    int value = engine.getTemperature();
    EXPECT_GE(value, -50);
    EXPECT_LE(value, 500);
    std::filesystem::remove("/tmp/test_engine_temp.db");
}

TEST(EngineTest, GetOilPressureReturnsValueInRange) {
    EngineImpl engine("/tmp/test_engine_oil.db");
    int value = engine.getOilPressure();
    EXPECT_GE(value, 0);
    EXPECT_LE(value, 200);
    std::filesystem::remove("/tmp/test_engine_oil.db");
}
TEST(EngineTest, GetSpeedReturnsValueInRange) {
    EngineImpl engine("/tmp/test_engine_speed.db");
    int value = engine.getSpeed();
    EXPECT_GE(value, 0);
    EXPECT_LE(value, 500);
    std::filesystem::remove("/tmp/test_engine_speed.db");
}

TEST(EngineTest, DatabaseIsCreated) {
    const std::string db_path = "/tmp/test_engine_db_created.db";
    std::filesystem::remove(db_path);
    
    EngineImpl engine(db_path);
    EXPECT_TRUE(std::filesystem::exists(db_path));
    
    std::filesystem::remove(db_path);
}

TEST(EngineTest, DatabaseTableExists) {
    const std::string db_path = "/tmp/test_engine_table.db";
    std::filesystem::remove(db_path);
    
    {
        EngineImpl engine(db_path);
        int rpm = engine.getRpm();
        int temp = engine.getTemperature();
        int oil = engine.getOilPressure();
        int speed = engine.getSpeed();
        engine.storeCurrentValues(rpm, temp, oil, speed);
    }
    
    sqlite3* db;
    int rc = sqlite3_open(db_path.c_str(), &db);
    ASSERT_EQ(rc, SQLITE_OK);
    
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='engine_values';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    ASSERT_EQ(rc, SQLITE_OK);
    
    rc = sqlite3_step(stmt);
    EXPECT_EQ(rc, SQLITE_ROW);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::filesystem::remove(db_path);
}

TEST(EngineTest, ValuesAreStoredInDatabase) {
    const std::string db_path = "/tmp/test_engine_values.db";
    std::filesystem::remove(db_path);
    
    {
        EngineImpl engine(db_path);
        // Store values multiple times
        for (int i = 0; i < 3; i++) {
            int rpm = engine.getRpm();
            int temp = engine.getTemperature();
            int oil = engine.getOilPressure();
            int speed = engine.getSpeed();
            engine.storeCurrentValues(rpm, temp, oil, speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    sqlite3* db;
    int rc = sqlite3_open(db_path.c_str(), &db);
    ASSERT_EQ(rc, SQLITE_OK);
    
    const char* query = "SELECT COUNT(*) FROM engine_values;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    ASSERT_EQ(rc, SQLITE_OK);
    
    rc = sqlite3_step(stmt);
    EXPECT_EQ(rc, SQLITE_ROW);
    int count = sqlite3_column_int(stmt, 0);
    EXPECT_GE(count, 3);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::filesystem::remove(db_path);
}

TEST(EngineTest, StoredValuesHaveTimestamp) {
    const std::string db_path = "/tmp/test_engine_timestamp.db";
    std::filesystem::remove(db_path);
    
    {
        EngineImpl engine(db_path);
        int rpm = engine.getRpm();
        int temp = engine.getTemperature();
        int oil = engine.getOilPressure();
        int speed = engine.getSpeed();
        engine.storeCurrentValues(rpm, temp, oil, speed);
    }
    
    sqlite3* db;
    int rc = sqlite3_open(db_path.c_str(), &db);
    ASSERT_EQ(rc, SQLITE_OK);
    
    const char* query = "SELECT timestamp FROM engine_values LIMIT 1;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    ASSERT_EQ(rc, SQLITE_OK);
    
    rc = sqlite3_step(stmt);
    EXPECT_EQ(rc, SQLITE_ROW);
    int64_t timestamp = sqlite3_column_int64(stmt, 0);
    EXPECT_GT(timestamp, 0);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::filesystem::remove(db_path);
}

TEST(EngineTest, StoredValuesAreInCorrectRange) {
    const std::string db_path = "/tmp/test_engine_range.db";
    std::filesystem::remove(db_path);
    
    {
        EngineImpl engine(db_path);
        int rpm = engine.getRpm();
        int temp = engine.getTemperature();
        int oil = engine.getOilPressure();
        int speed = engine.getSpeed();
        engine.storeCurrentValues(rpm, temp, oil, speed);
    }
    
    sqlite3* db;
    int rc = sqlite3_open(db_path.c_str(), &db);
    ASSERT_EQ(rc, SQLITE_OK);
    
    const char* query = "SELECT rpm, temperature, oil_pressure, speed FROM engine_values LIMIT 1;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    ASSERT_EQ(rc, SQLITE_OK);
    
    rc = sqlite3_step(stmt);
    EXPECT_EQ(rc, SQLITE_ROW);
    
    int rpm = sqlite3_column_int(stmt, 0);
    int temperature = sqlite3_column_int(stmt, 1);
    int oil_pressure = sqlite3_column_int(stmt, 2);
    int speed = sqlite3_column_int(stmt, 3);

    EXPECT_GE(rpm, 0);
    EXPECT_LE(rpm, 8000);
    EXPECT_GE(temperature, -50);
    EXPECT_LE(temperature, 500);
    EXPECT_GE(oil_pressure, 0);
    EXPECT_LE(oil_pressure, 200);
    EXPECT_GE(speed, 0);
    EXPECT_LE(speed, 500);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::filesystem::remove(db_path);
}
