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

## Run

- Run the main application (builds if needed):
  ```bash
  ./run_app.sh <UpdateIntervalMs>
  ```

- Run unit tests:
  ```bash
  ./run_tests.sh
  ```

## Socket Client Example

You can use the provided Python client to connect to the app's socket server (port 5555) and receive live engine data every second:

1. **Create and activate a virtual environment:**
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   ```

2. **Install dependencies:**
   ```bash
   pip install protobuf
   ```

```bash
python3 test_client.py
```
The client will print the latest RPM and temperature values received from the server. You can stop the client with Ctrl+C.

