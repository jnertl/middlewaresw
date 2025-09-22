
# Project Features and Requirements

## Features

### [REQ001] C++ Application
- Uses CMake for build configuration
- Follows C++17 standard or later

### [REQ002] Interface
- Provies Socket Server TCP port 5555 for clients
- On request, sends latest engine data as a Protocol Buffers message (with size prefix)

### [REQ003] Protocol Buffers Integration
- `engine_data.proto` defines the message format:
   - `int32 rpm`
   - `int32 temperature`
   - `int32 oil_pressure`

### [REQ004] Engine Data Simulation
- `Receiver` class provides random RPM and temperature values within defined ranges
- `Engine` interface class with pure virtual methods for RPM and temperature
- `EngineImpl` class implements `Engine` and uses `Receiver` for data

## Requirements for Testing

### [REQ100] Debug Support
- Application periodically prints engine RPM, oil pressure and temperature to the console

### [REQ101] Unit Tests
- Test all classes and methods using GoogleTest

### [REQ102] Coverage Reports
- Generate code coverage reports using lcov
- Coverage reports should reach a minimum threshold (90%)

## Other Requirements

### [REQ200] Thread Safety
All shared data accessed by multiple threads (e.g., engine data in Server) must be protected by mutexes to prevent race conditions.

### [REQ201] Graceful Shutdown
The application and server threads should support clean shutdown on user interrupt (e.g., SIGINT/Ctrl+C).

### [REQ202] Error Handling
All socket and system calls should have robust error handling and logging.

### [REQ203] Extensible Data Model
The engine data model should be easily extensible (e.g., adding new fields to EngineData proto and corresponding code).

### [REQ204] Client Compatibility
The Python client and any other clients must be compatible with the current protobuf message format.

### [REQ205] Build Automation
The build and test scripts should work on a clean environment with minimal manual setup.

### [REQ206] Documentation
The README and code should be kept up to date with all features, requirements, and usage instructions.

### [REQ207] Resource Management
All sockets and threads should be properly closed and joined to avoid resource leaks.

## Requirements for Environment Setup

- Python 3.7+ and PyQt5 (for client and/or other scripts)
- GoogleTest (gtest) for C++ unit tests
- Protocol Buffers (protobuf) C++ library and compiler
- lcov (for coverage reports)
- CMake >= 3.10
- Standard Linux build tools (gcc/g++, make, etc.)
