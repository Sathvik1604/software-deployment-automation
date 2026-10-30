/**
 * @file calculator.h
 * @brief Header declaration for the C++ Calculator module.
 * @project C++ Software Deployment Automation with Workflow Reporting
 * 
 * Declares arithmetic functions and operation handler.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace Calculator {

/**
 * @brief Adds two numbers.
 * @param a First operand
 * @param b Second operand
 * @return Sum of a and b
 */
double add(double a, double b);

/**
 * @brief Subtracts second number from first number.
 * @param a First operand
 * @param b Second operand
 * @return Difference (a - b)
 */
double subtract(double a, double b);

/**
 * @brief Multiplies two numbers.
 * @param a First operand
 * @param b Second operand
 * @return Product of a and b
 */
double multiply(double a, double b);

/**
 * @brief Divides first number by second number.
 * @param a Numerator
 * @param b Denominator (must not be zero)
 * @return Quotient (a / b)
 * @throws std::invalid_argument if denominator b is zero
 */
double divide(double a, double b);

/**
 * @brief Dispatches calculation based on arithmetic operator character.
 * @param a First operand
 * @param op Arithmetic operator: '+', '-', '*', '/' (or 'x', 'X')
 * @param b Second operand
 * @return Result of the operation
 * @throws std::invalid_argument for division by zero or unsupported operator
 */
double calculate(double a, char op, double b);

} // namespace Calculator
