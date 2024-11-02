#include <iostream>
#include "../Calculator/Calculator.h"

int main() {
    Calculator::Calculator calc;
    std::string expression;
    std::cout << "Enter expression: ";
    std::getline(std::cin, expression);

    try {
        double result = calc.calculate(expression);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

