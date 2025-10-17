# middlewaresw

## Overview
A C++ middleware application that simulates engine data and provides it to clients via a TCP socket server using Protocol Buffers serialization.

## Features
- CMake-based build system (requires CMake >= 3.10)
- C++17 or later required
- TCP socket server listens on port 5555 (IPv4, INADDR_ANY)
- On client request, sends latest engine data as a Protocol Buffers message, prefixed by a 4-byte big-endian size
- Engine data includes: RPM (600-7000), temperature (70-120°C), oil pressure (psi)
- `Receiver` class generates random RPM and temperature values in defined ranges
- `Engine` interface class declares pure virtual methods for `getRpm()`, `getTemperature()`, and `getOilPressure()`
- `EngineImpl` implements `Engine` and uses `Receiver` for data
- **Stored values with timestamps**: Each value retrieved from the receiver is automatically stored with a timestamp in `EngineImpl`, accessible via `getStoredRpm()`, `getStoredTemperature()`, and `getStoredOilPressure()` methods
- All shared data accessed by multiple threads is protected by mutexes
- Graceful shutdown on SIGINT (Ctrl+C): all threads joined, sockets closed, shutdown message printed
- Robust error handling: all socket and system calls check for errors and log descriptive messages
- Engine data model is extensible via `engine_data.proto` (add new fields with minimal changes)
- Python client and other clients compatible with current protobuf message format
- Unit tests for all public classes and methods using GoogleTest (gtest)
- Code coverage reports generated with lcov (>=80% line coverage required)
- Build and test scripts work on a clean Linux environment
- Comprehensive documentation in README and code comments
- No resource leaks: all sockets closed, threads joined before exit

## Requirements
- Python 3.7 or newer, with PyQt5 (for client and/or other scripts)
- GoogleTest (gtest) for C++ unit tests
- Protocol Buffers (protobuf) C++ library and compiler
- lcov (for coverage reports)
- CMake >= 3.10
- Standard Linux build tools (gcc, g++, make, etc.)

Install dependencies:
```bash
sudo apt-get install libprotobuf-dev protobuf-compiler lcov
```

If you modify `engine_data.proto`, regenerate the C++ files:
```bash
protoc --cpp_out=include engine_data.proto
```

## Build Instructions
Build main application:
```bash
./build.sh
```

## Unit Testing
Run unit tests:
```bash
./run_tests.sh
```

## Run
Run the main application (builds if needed):
```bash
./run_app.sh <UpdateIntervalMs>
```

## TCP Socket Client Example
You can use the provided Python client to connect to the socket server (port 5555) and receive live engine data:

1. Create and activate a virtual environment:
  ```bash
  python3 -m venv test_client_venv
  source test_client_venv/bin/activate
  ```
2. Install dependencies:
  ```bash
  pip install protobuf
  ```
3. Run test client:
  ```bash
  python3 test_client.py
  ```
The client will print the latest values received from the server. Stop the client with Ctrl+C.

## Debug Output
During normal operation, the application prints engine RPM, oil pressure, and temperature to the console at least every 200ms:
```
Engine RPM:[<rpm>], Temperature:[<temperature>], Oil Pressure:[<oil_pressure>] psi
```

## Graceful Shutdown
Press Ctrl+C to stop the application. All threads will be joined, sockets closed, and a shutdown message printed.

## Error Handling
All socket and system calls check for errors and print descriptive error messages. The application does not crash on recoverable errors.

## Extensibility
To add new fields to engine data, update `engine_data.proto` and regenerate C++ files. Update client code and README as needed.

## Stored Values with Timestamps
The `EngineImpl` class automatically stores each value retrieved from the receiver along with its timestamp. This feature enables tracking when each measurement was taken:

- **Storage**: Each call to `getRpm()`, `getTemperature()`, or `getOilPressure()` stores the value and current timestamp
- **Retrieval**: Use `getStoredRpm()`, `getStoredTemperature()`, or `getStoredOilPressure()` to get a `StoredValue` struct containing:
  - `int value`: The measured value
  - `std::chrono::system_clock::time_point timestamp`: When the value was obtained
- **Thread Safety**: All storage operations are protected by a mutex, ensuring safe concurrent access from multiple threads
- **Use Cases**: Historical tracking, timing analysis, debugging, and correlation of measurements

Example usage:
```cpp
EngineImpl engine;
engine.getRpm(); // Fetches and stores RPM with timestamp
StoredValue stored = engine.getStoredRpm();
// Access: stored.value (the RPM), stored.timestamp (when it was measured)
```

## Test Coverage for C++
To build and view a C++ code coverage report:
1. Install lcov (if not already):
  ```bash
  sudo apt-get install lcov
  ```
2. Run the coverage script:
  ```bash
  ./run_coverage.sh
  ```
3. Open the generated HTML report:
  ```bash
  xdg-open coverage_html/index.html
  ```

## Resource Management
All sockets are closed after use. All threads are joined before application exit. No resource leaks are permitted.

## Logging (spdlog)

This project uses [spdlog](https://github.com/gabime/spdlog) for fast, header-only logging with timestamps.

- spdlog is included as a git submodule in `external/spdlog`.
- The `.gitmodules` file tracks the submodule:

**To initialize the submodule after cloning:**
```bash
git submodule update --init --recursive
```
