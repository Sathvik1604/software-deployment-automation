cmake_minimum_required(VERSION 3.16)

# Project Definition
project(
    CppCalculatorAutomation
    VERSION 1.0.0
    DESCRIPTION "Academic C++ deployment automation demo with CTest and GitHub Actions"
    LANGUAGES CXX
)

# Standard C++17 Specification
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Warnings (Compatible with GCC and Apple Clang)
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# -----------------------------------------------------------------------------
# Static Library: Core Calculator Logic
# Reused by both the production executable and the automated test suite
# -----------------------------------------------------------------------------
add_library(calculator_lib STATIC
    src/calculator.cpp
    src/calculator.h
)

target_include_directories(calculator_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -----------------------------------------------------------------------------
# Main Application Executable
# -----------------------------------------------------------------------------
add_executable(calculator
    src/main.cpp
)

target_link_libraries(calculator PRIVATE
    calculator_lib
)

# -----------------------------------------------------------------------------
# Automated Testing Suite (CTest)
# -----------------------------------------------------------------------------
enable_testing()

add_executable(test_calculator
    tests/test_calculator.cpp
)

target_link_libraries(test_calculator PRIVATE
    calculator_lib
)

# Register test suite with CTest
add_test(
    NAME CalculatorTestSuite
    COMMAND test_calculator
)
