#include "Engine.h"
#include <chrono>
#include <spdlog/spdlog.h>

// Static variables for simulating a crash in getRpm()
static uint32_t untilCrashCounter = 0;
static const bool crashEnabled = false;

EngineImpl::EngineImpl() : db(nullptr) {
    initDatabase("engine_data.db");
}

EngineImpl::EngineImpl(const std::string& db_path) : db(nullptr) {
    initDatabase(db_path);
}

EngineImpl::~EngineImpl() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void EngineImpl::initDatabase(const std::string& db_path) {
    int rc = sqlite3_open(db_path.c_str(), &db);
    if (rc != SQLITE_OK) {
        spdlog::error("Cannot open database: {}", sqlite3_errmsg(db));
        sqlite3_close(db);
        db = nullptr;
        return;
    }

    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS engine_values ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "rpm INTEGER NOT NULL,"
        "temperature INTEGER NOT NULL,"
        "oil_pressure INTEGER NOT NULL,"
        "timestamp INTEGER NOT NULL"
        ");";

    char* err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        spdlog::error("SQL error: {}", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        db = nullptr;
    }
}

void EngineImpl::storeCurrentValues(int rpm, int temperature, int oil_pressure) {
    if (!db) {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();

    const char* insert_sql = 
        "INSERT INTO engine_values (rpm, temperature, oil_pressure, timestamp) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        spdlog::error("Failed to prepare statement: {}", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, rpm);
    sqlite3_bind_int(stmt, 2, temperature);
    sqlite3_bind_int(stmt, 3, oil_pressure);
    sqlite3_bind_int64(stmt, 4, timestamp);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        spdlog::error("Failed to execute statement: {}", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

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
