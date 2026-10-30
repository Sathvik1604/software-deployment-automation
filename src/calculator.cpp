/**
 * @file calculator.cpp
 * @brief Implementation of the C++ Calculator arithmetic logic.
 * @project C++ Software Deployment Automation with Workflow Reporting
 * 
 * Implements addition, subtraction, multiplication, safe division,
 * and operator dispatching.
 */

#include "calculator.h"
#include <cmath>

namespace Calculator {

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    // Check for division by zero using floating-point tolerance
    if (std::abs(b) < 1e-9) {
        throw std::invalid_argument("Division by zero error: denominator cannot be zero.");
    }
    return a / b;
}

double calculate(double a, char op, double b) {
    switch (op) {
        case '+':
            return add(a, b);
        case '-':
            return subtract(a, b);
        case '*':
        case 'x':
        case 'X':
            return multiply(a, b);
        case '/':
            return divide(a, b);
        default:
            throw std::invalid_argument(
                std::string("Invalid operator '") + op + "'. Supported operators: +, -, *, /"
            );
    }
}

} // namespace Calculator
