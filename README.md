# middlewaresw

## Overview
A C++ project demonstrating a periodic console print.

## Features
- `main.cpp` accepts an integer argument `UpdateIntervalMs` (milliseconds)
- `Receiver` class: provides `GetRpm()` and `GetTemperature()` methods returning random values in defined ranges
- `Engine` interface class: declares `getRpm()` and `getTemperature()` as pure virtual methods
- `EngineImpl` implements `Engine` and uses `Receiver` to provide RPM and temperature values
- Main loop in `main.cpp` calls `Receiver` methods periodically based on `UpdateIntervalMs`
- Unit tests for both `Receiver` and `EngineImpl` using Google Test (gtest)
- Build script (`build.sh`) for building and cleaning the project
- Test script (`run_tests.sh`) for running all unit tests

## Build Instructions

## Requirements
- Python 3.7+
- PyQt5
- sudo apt install python3-pip python3-venv 
- GoogleTest (gtest) must be installed  
- Protocol Buffers (protobuf) C++ library and compiler must be installed:
  ```bash
  sudo apt-get install libprotobuf-dev protobuf-compiler
  ```
  
  The project uses `engine_data.proto` to generate `engine_data.pb.h` and `engine_data.pb.cc` (already included in the repo). If you modify `engine_data.proto`, regenerate these files with:
  ```bash
  protoc --cpp_out=include engine_data.proto
  ```
  (You may need to adjust the output path if your structure changes.)

- Build main application:
  ```bash
  ./build.sh
  ```

## Unit testing
- Run unit tests:
  ```bash
  ./run_tests.sh
  ```

## Run

- Run the main application (builds if needed):
  ```bash
  ./run_app.sh <UpdateIntervalMs>
  ```

## Module testing

You can use the provided Python client to connect to the app's socket server (port 5555) and receive live engine data:

1. **Create and activate a virtual environment:**
   ```bash
   python3 -m venv test_client_venv
   source test_client_venv/bin/activate
   ```

2. **Install dependencies:**
   ```bash
   pip install protobuf
   ```

3. **Run test client:**
```bash
python3 test_client.py
```
The client will print the latest values received from the server. You can stop the client with Ctrl+C.

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