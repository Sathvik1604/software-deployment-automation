/**
 * @file test_calculator.cpp
 * @brief Automated test suite for C++ Calculator application.
 * @project C++ Software Deployment Automation with Workflow Reporting
 * 
 * Tests core arithmetic operations and error handling.
 * Integrates natively with CTest via CMake.
 */

#include "../src/calculator.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include <string>

// Global test counters
static int g_testsPassed = 0;
static int g_testsFailed = 0;

void assertNear(double actual, double expected, double tolerance, const std::string& testName) {
    if (std::abs(actual - expected) <= tolerance) {
        std::cout << "  [PASS] " << testName 
                  << " (Expected: " << expected << ", Actual: " << actual << ")\n";
        g_testsPassed++;
    } else {
        std::cerr << "  [FAIL] " << testName 
                  << " (Expected: " << expected << ", Actual: " << actual << ")\n";
        g_testsFailed++;
    }
}

// 1. Addition of two positive numbers
void testAdditionPositive() {
    std::cout << "[RUN] Test 1: Addition of two positive numbers\n";
    double res = Calculator::add(15.5, 24.5);
    assertNear(res, 40.0, 1e-6, "Calculator::add(15.5, 24.5)");
}

// 2. Addition involving a negative number
void testAdditionNegative() {
    std::cout << "[RUN] Test 2: Addition involving a negative number\n";
    double res1 = Calculator::add(-10.0, 30.0);
    assertNear(res1, 20.0, 1e-6, "Calculator::add(-10.0, 30.0)");

    double res2 = Calculator::add(-15.0, -25.0);
    assertNear(res2, -40.0, 1e-6, "Calculator::add(-15.0, -25.0)");
}

// 3. Subtraction
void testSubtraction() {
    std::cout << "[RUN] Test 3: Subtraction\n";
    double res1 = Calculator::subtract(50.0, 15.0);
    assertNear(res1, 35.0, 1e-6, "Calculator::subtract(50.0, 15.0)");

    double res2 = Calculator::subtract(10.0, 25.0);
    assertNear(res2, -15.0, 1e-6, "Calculator::subtract(10.0, 25.0)");
}

// 4. Multiplication
void testMultiplication() {
    std::cout << "[RUN] Test 4: Multiplication\n";
    double res1 = Calculator::multiply(6.0, 7.0);
    assertNear(res1, 42.0, 1e-6, "Calculator::multiply(6.0, 7.0)");

    double res2 = Calculator::multiply(-4.0, 5.0);
    assertNear(res2, -20.0, 1e-6, "Calculator::multiply(-4.0, 5.0)");

    double res3 = Calculator::multiply(100.0, 0.0);
    assertNear(res3, 0.0, 1e-6, "Calculator::multiply(100.0, 0.0)");
}

// 5. Division
void testDivision() {
    std::cout << "[RUN] Test 5: Division\n";
    double res1 = Calculator::divide(100.0, 4.0);
    assertNear(res1, 25.0, 1e-6, "Calculator::divide(100.0, 4.0)");

    double res2 = Calculator::divide(7.0, 2.0);
    assertNear(res2, 3.5, 1e-6, "Calculator::divide(7.0, 2.0)");
}

// 6. Division by zero exception handling
void testDivisionByZero() {
    std::cout << "[RUN] Test 6: Division by zero exception\n";
    bool exceptionCaught = false;
    try {
        Calculator::divide(50.0, 0.0);
    } catch (const std::invalid_argument& ex) {
        exceptionCaught = true;
        std::cout << "  [PASS] Expected exception caught: " << ex.what() << "\n";
    }

    if (exceptionCaught) {
        g_testsPassed++;
    } else {
        std::cerr << "  [FAIL] Calculator::divide(50.0, 0.0) did not throw std::invalid_argument\n";
        g_testsFailed++;
    }
}

// 7. Handling of invalid operations
void testInvalidOperations() {
    std::cout << "[RUN] Test 7: Handling of invalid operations\n";
    bool exceptionCaught = false;
    try {
        Calculator::calculate(10.0, '%', 5.0);
    } catch (const std::invalid_argument& ex) {
        exceptionCaught = true;
        std::cout << "  [PASS] Unsupported operator caught: " << ex.what() << "\n";
    }

    if (exceptionCaught) {
        g_testsPassed++;
    } else {
        std::cerr << "  [FAIL] Calculator::calculate with invalid operator did not throw\n";
        g_testsFailed++;
    }
}

int main() {
    std::cout << "====================================================\n";
    std::cout << " Starting C++ Calculator Automated Test Suite (CTest)\n";
    std::cout << "====================================================\n";

    testAdditionPositive();
    testAdditionNegative();
    testSubtraction();
    testMultiplication();
    testDivision();
    testDivisionByZero();
    testInvalidOperations();

    std::cout << "====================================================\n";
    std::cout << " Test Summary: " << g_testsPassed << " Passed, " 
              << g_testsFailed << " Failed\n";
    std::cout << "====================================================\n";

    if (g_testsFailed > 0) {
        std::cerr << "[TEST SUITE FAILED] One or more test assertions failed.\n";
        return 1;
    }

    std::cout << "[TEST SUITE PASSED] All tests executed successfully!\n";
    return 0;
}
