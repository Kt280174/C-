#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "../Calculator/Importer.h"
#include "../Calculator/Node.h"
#define _USE_MATH_DEFINES
#include <map>
#include <queue>
#include <stack>
#include <math.h>
#include <string>
#include <stdexcept>

namespace Calculator {
    class Calculator {
    private:
        Importer importer; // Importer instance to load plugins
        std::map<std::string, double> constants = {
            {"pi", M_PI},
            {"e", M_E}
        };

        std::map<std::string, Functions> unaryFunctions;
        std::map<std::string, Functions> binaryFunctions;

        // Helper functions for calculation
        bool isNumber(const std::string& token);
        int getPrecedence(const std::string& op);
        bool isLeftAssociative(const std::string& op);
        std::queue<std::string> toRPN(const std::string& expression);
        Node* parseRPN(std::queue<std::string>& rpnQueue);
        double evaluate(Node* node);

    public:
        Calculator();
        double calculate(const std::string& expression);
    };
}
#endif // CALCULATOR_H
