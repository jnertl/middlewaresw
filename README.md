# middlewaresw_create

## Overview
A C++ project demonstrating a periodic receiver with Google Test support.

## Features
- `main.cpp` accepts an integer argument `UpdateIntervalMs` (milliseconds)
- `Receiver` class: `Get()` stores a random integer (0-300)
- `Engine` interface class: `getSpeed()` returns an integer
- Main loop calls `Receiver::Get()` periodically based on `UpdateIntervalMs`
- Unit tests using Google Test (gtest)

## Build Instructions

1. Clone GoogleTest into the `tests` directory:
   ```bash
   git clone https://github.com/google/googletest.git tests/lib/googletest
   ```
2. Build the project:
   ```bash
   mkdir -p build && cd build
   cmake ..
   make
   ```

## Run

- Run the main application:
  ```bash
  ./middlewaresw_create <UpdateIntervalMs>
  ```
- Run unit tests:
  ```bash
  cd build
  ctest
  ```
