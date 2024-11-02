#ifndef FUNCTION_H
#define FUNCTION_H
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include <functional>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include "../Calculator/TokenType.h"
// Function class to encapsulate each operator or function

namespace Calculator {

    class Functions {
    public:
        std::function<double(double, double)> eval; // Function pointer for evaluation
        TokenType type; // Use of the TokenType enum class
        int precedence; // Precedence of the operator/function
        bool leftAssociative; // Associativity of the operator/function

        // Constructor for Function class
        Functions(std::function<double(double, double)> evalFunc, TokenType type = TokenType::Function, int precedence = 0, bool leftAssociative = true)
            : eval(evalFunc), type(type), precedence(precedence), leftAssociative(leftAssociative) {}

        // Default constructor
        Functions() : eval(nullptr), type(TokenType::Unknown), precedence(0), leftAssociative(true) {}
    };

} // namespace Calculator

#endif // FUNCTION_H

