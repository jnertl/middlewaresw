

# Project Features and Requirements 

## Features

### [REQ001] C++ Application
- The application must use CMake for build configuration.
- The codebase must compile and run with C++17 standard or later. Compilation must succeed with `-std=c++17` or higher.

### [REQ002] TCP Socket Server Interface
- The application must provide a TCP socket server listening on port 5555 (IPv4, INADDR_ANY).
- The server must accept multiple client connections sequentially (one at a time).
- Upon receiving any data from a connected client, the server must respond with the latest engine data as a Protocol Buffers message, prefixed by a 4-byte big-endian message size.
- The server must log connection, disconnection, and message send events to the console.

### [REQ003] Protocol Buffers Integration
- The file `engine_data.proto` must define a message with the following fields:
   - `int32 rpm` (engine revolutions per minute)
   - `int32 temperature` (engine temperature in degrees Celsius)
   - `int32 oil_pressure` (oil pressure in psi)
- The generated C++ classes from this proto file must be used for all engine data serialization and deserialization.

### [REQ004] Engine Data Simulation
- The `Receiver` class must generate random RPM values in the range [600, 7000] and temperature values in the range [70, 120].
- The `Engine` interface class must declare pure virtual methods for `getRpm()` and `getTemperature()`.
- The `EngineImpl` class must implement `Engine` and use a `Receiver` instance to provide data.

## Testing Requirements

### [REQ100] Debug Output
- The application must print the current engine RPM, oil pressure, and temperature to the console at least once every 200 milliseconds during normal operation.
- Output format must be: `Engine RPM:[<rpm>], Temperature:[<temperature>], Oil Pressure:[<oil_pressure>] psi`

### [REQ101] Unit Tests
- All public classes and methods must have corresponding unit tests implemented using GoogleTest.
- All tests must pass when run with the provided test script.

### [REQ102] Coverage Reports
- Code coverage reports must be generated using lcov and must include all source files except test code.
- Coverage must reach at least 80% line coverage for all major classes and functions.

## Other Requirements

### [REQ200] Thread Safety
- All shared data accessed by multiple threads (e.g., engine data in `Server`) must be protected by a `std::mutex` or equivalent locking mechanism.
- All read and write operations to shared variables must use lock guards to prevent race conditions.

### [REQ201] Graceful Shutdown
- The application must handle SIGINT (Ctrl+C) and perform a clean shutdown:
   - The main loop must exit.
   - All server and data update threads must be joined.
   - All sockets must be closed.
   - A shutdown message must be printed to the console.

### [REQ202] Error Handling
- All socket and system calls must check for errors and print descriptive error messages to the console.
- The application must not crash on recoverable errors; it must log and continue or exit gracefully.

### [REQ203] Extensible Data Model
- The engine data model must allow new fields to be added to `engine_data.proto` and corresponding C++ code with minimal changes (no breaking changes for existing clients).

### [REQ204] Client Compatibility
- The Python client and any other clients must be able to parse the current Protocol Buffers message format and receive engine data from the server without errors.
- Any changes to the message format must be reflected in the client code and documented in the README.

### [REQ205] Test Coverage
- All new classes and major functions must have corresponding unit tests.
- Coverage reports must reach at least 80% line coverage for all major classes and functions.

### [REQ206] Build Automation
- The build and test scripts must work on a clean Linux environment with no manual setup except installing required dependencies.
- Scripts must print clear error messages if any step fails.

### [REQ207] Documentation
- The README file must describe all features, requirements, usage instructions, and environment setup steps.
- All code must include comments explaining non-obvious logic and design decisions.

### [REQ208] Resource Management
- All sockets must be closed after use.
- All threads must be joined before application exit.
- No resource leaks (memory, file descriptors, threads) are permitted.

## Environment Setup Requirements

- Python 3.7 or newer, with PyQt5 installed (for client and/or other scripts)
- GoogleTest (gtest) for C++ unit tests
- Protocol Buffers (protobuf) C++ library and compiler
- lcov (for coverage reports)
- CMake version 3.10 or newer
- Standard Linux build tools (gcc, g++, make, etc.)
