/**
 * @file main.cpp
 * @brief Entry point for the C++ Calculator application.
 * @project C++ Software Deployment Automation with Workflow Reporting
 * 
 * Supports both CLI arguments (e.g., ./calculator 10 + 5)
 * and automated demonstration mode (when executed without arguments),
 * perfectly suited for automated Docker verification.
 */

#include "calculator.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

void printUsage(const char* progName) {
    std::cout << "====================================================\n";
    std::cout << " C++ Calculator - Deployment Automation Project\n";
    std::cout << "====================================================\n";
    std::cout << "Usage:\n";
    std::cout << "  " << progName << " <operand1> <operator> <operand2>\n\n";
    std::cout << "Supported operators: + , - , * , /\n";
    std::cout << "Examples:\n";
    std::cout << "  " << progName << " 15 + 7\n";
    std::cout << "  " << progName << " 20 * 4\n";
    std::cout << "  " << progName << " 100 / 5\n";
    std::cout << "====================================================\n";
}

int runDeterministicDemo() {
    std::cout << "====================================================\n";
    std::cout << " C++ Calculator: Automated Container Demonstration  \n";
    std::cout << "====================================================\n";
    std::cout << "[STATUS] Running built-in deterministic verification suite...\n\n";

    struct TestCase {
        double a;
        char op;
        double b;
        std::string label;
    };

    std::vector<TestCase> cases = {
        {25.0, '+', 15.0, "Addition (25 + 15)"},
        {50.0, '-', 18.5, "Subtraction (50 - 18.5)"},
        {12.0, '*', 8.0,  "Multiplication (12 * 8)"},
        {144.0, '/', 12.0, "Division (144 / 12)"}
    };

    for (const auto& tc : cases) {
        try {
            double res = Calculator::calculate(tc.a, tc.op, tc.b);
            std::cout << "[EXECUTE] " << tc.label << " => Result: " 
                      << std::fixed << std::setprecision(2) << res << "\n";
        } catch (const std::exception& ex) {
            std::cerr << "[ERROR] " << tc.label << " failed: " << ex.what() << "\n";
            return 1;
        }
    }

    // Explicitly test and catch division by zero handling in demo
    std::cout << "\n[EXECUTE] Testing Division by Zero Protection (10 / 0):\n";
    try {
        Calculator::calculate(10.0, '/', 0.0);
        std::cerr << "[FAIL] Division by zero did not raise an exception!\n";
        return 1;
    } catch (const std::invalid_argument& ex) {
        std::cout << "[SUCCESS] Caught expected safety exception: \"" 
                  << ex.what() << "\"\n";
    }

    std::cout << "\n====================================================\n";
    std::cout << "[VERIFICATION COMPLETE] All operations executed successfully.\n";
    std::cout << "Exit code: 0 (SUCCESS)\n";
    std::cout << "====================================================\n";
    return 0;
}

int main(int argc, char* argv[]) {
    // If run without arguments (e.g. inside Docker automated container run),
    // execute the deterministic verification test suite.
    if (argc == 1) {
        return runDeterministicDemo();
    }

    // If help flag requested
    if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
        printUsage(argv[0]);
        return 0;
    }

    // If 3 arguments provided: <num1> <op> <num2>
    if (argc == 4) {
        try {
            double a = std::stod(argv[1]);
            std::string opStr = argv[2];
            double b = std::stod(argv[3]);

            if (opStr.length() != 1) {
                std::cerr << "[ERROR] Operator must be a single character (+, -, *, /).\n";
                return 2;
            }

            char op = opStr[0];
            double result = Calculator::calculate(a, op, b);

            std::cout << "[RESULT] " << a << " " << op << " " << b << " = " 
                      << std::fixed << std::setprecision(4) << result << "\n";
            return 0;
        } catch (const std::invalid_argument& ex) {
            std::cerr << "[ERROR] Invalid argument: " << ex.what() << "\n";
            return 1;
        } catch (const std::out_of_range& ex) {
            std::cerr << "[ERROR] Numerical value out of range.\n";
            return 1;
        } catch (const std::exception& ex) {
            std::cerr << "[ERROR] Unexpected error: " << ex.what() << "\n";
            return 1;
        }
    }

    // Invalid argument count
    std::cerr << "[ERROR] Incorrect number of arguments.\n";
    printUsage(argv[0]);
    return 2;
}
