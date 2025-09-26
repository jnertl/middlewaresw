

# Project Features and Requirements 

## Features

### [REQ001] C++ Application
- The application must use CMake version 3.28 or newer for build configuration.
  Use cmake_minimum_required(VERSION 3.28) in makefiles.
- The codebase must compile and run with C++20 standard or later. Compilation must succeed with `-std=c++20` or higher.
  Use set(CMAKE_CXX_STANDARD_REQUIRED ON) in makefiles.

### [REQ002] TCP Socket Server Interface
- The application must provide a TCP socket server listening on port 5555 (IPv4, INADDR_ANY).
- The server must accept multiple client connections sequentially (one at a time).
- Upon receiving any data from a connected client, the server must respond with the latest engine data as a Protocol Buffers message, prefixed by a 4-byte big-endian message size.
- The server must log connection, disconnection, and message send events to the console.

### [REQ003] Protocol Buffers Integration
- The file `engine_data.proto` must use Protocol Buffers syntax version 3 and define the following message:

```proto
syntax = "proto3";

message EngineData {
   int32 rpm = 1;           // Engine revolutions per minute
   int32 temperature = 2;   // Engine temperature in degrees Celsius
   int32 oil_pressure = 3;  // Oil pressure in psi
}
```

- The generated C++ classes from this proto file must be used for all engine data serialization and deserialization.

### [REQ004] Engine Data Simulation
- The `Receiver` class must generate random RPM values in the range [600, 7000] and temperature values in the range [70, 120].
- The `Engine` interface class must declare pure virtual methods for `getRpm()` and `getTemperature()`.
- The `EngineImpl` class must implement `Engine` and use a `Receiver` instance to provide data.

## Testing Requirements

### [REQ100] Debug Output
- The application must print the current engine RPM, oil pressure, and temperature to the standard output (console) at least once every 200 milliseconds during normal operation.
- The output string must exactly match:
   `Engine RPM:[<rpm>], Temperature:[<temperature>], Oil Pressure:[<oil_pressure>] psi`
   where `<rpm>`, `<temperature>`, and `<oil_pressure>` are integer values representing the latest engine data.
- Each output line must be printed on a new line, with no extra whitespace before or after the string.
- The output must be visible in the terminal when running the application interactively.

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
- The server must use std::cout to print connection, disconnection, and message send events to the console.
- All socket and system calls must check for errors and print descriptive error messages to the console, including the function name, error code, and a human-readable description.
- Recoverable errors (e.g., temporary network failures, client disconnects, resource exhaustion) must be handled by:
   - Logging the error with sufficient detail for debugging.
   - Attempting to retry the operation if appropriate (e.g., transient network errors).
   - Cleaning up resources (e.g., closing sockets, releasing memory) before continuing.
   - Notifying the user or administrator if the error affects application functionality.
- The application must not crash or terminate unexpectedly on recoverable errors; it must log the error and either continue operation or exit gracefully with a clear message.
- Example recoverable error handling:
   - If a client disconnects unexpectedly, log the event and continue accepting new clients.
   - If a socket read/write fails due to a transient error, log the error and retry up to a defined limit before giving up.
   - If memory allocation fails, log the error and attempt to free unused resources before retrying or exiting gracefully.

### [REQ203] Extensible Data Model
- The engine data model must allow new fields to be added to `engine_data.proto` and corresponding C++ code with minimal changes (no breaking changes for existing clients).

### [REQ204] Client Compatibility
- The Python client and any other clients must be able to parse the current Protocol Buffers message format and receive engine data from the server without errors.
- Any changes to the Protocol Buffers message format (e.g., adding, removing, or renaming fields in `engine_data.proto`) must be:
   - Documented in the README file, including the new message definition and field descriptions.
   - Reflected in all client code (Python and others) to ensure compatibility.
   - Noted in the project changelog or release notes, if applicable.

### [REQ205] Test Coverage
- All new classes and major functions must have corresponding unit tests implemented using GoogleTest.
- Unit tests must cover:
   - All public methods and constructors
   - Typical usage scenarios
   - Edge cases and error conditions
- Use assertions to verify correct behavior, output, and error handling.
- Coverage reports must reach at least 80% line coverage for all major classes and functions, as measured by lcov.
- Coverage measurement must exclude test code and focus on application logic.
- If coverage falls below the threshold, add additional tests to cover untested branches, conditions, and error paths.
- Coverage reports must be generated after each major change and included in project documentation or CI reports.

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
