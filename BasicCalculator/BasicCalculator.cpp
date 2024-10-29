// main.cpp
#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <windows.h>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <stack>

typedef double (*SinFunction)(double);
typedef double (*PowerFunction)(double, double);

double calculate_power(double base, double exponent, HMODULE hModule) {
    PowerFunction powerFunc = (PowerFunction)GetProcAddress(hModule, "power_function");
    if (!powerFunc) {
        throw std::runtime_error("Failed to load power_function from DLL");
    }
    return powerFunc(base, exponent);
}

double calculate_sin(double angle, HMODULE hModule) {
    SinFunction sinFunc = (SinFunction)GetProcAddress(hModule, "sin_function");
    if (!sinFunc) {
        throw std::runtime_error("Failed to load sin_function from DLL");
    }
    return sinFunc(angle);
}

bool isSin(const std::string& token) {
    return token == "sin";
 }

int getPrecedence(const std::string& op) {
    if (op == "sin")
        return 4;
    if (op == "^")
        return 3;
    if (op == "*" || op == "/")
        return 2;
    if (op == "+" || op == "-")
        return 1;
    return 0;
}

bool isRightAssociative(const std::string& op) {
    if (op == "^")
        return true;
    return false;
}

std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    size_t i = 0;
    while (i < expr.length()) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }
        if (isdigit(expr[i]) || expr[i] == '.') {
            std::string num;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                num += expr[i++];
            }
            tokens.push_back(num);
        }
        else if (isalpha(expr[i])) {
            std::string func;
            while (i < expr.length() && isalpha(expr[i])) {
                func += expr[i++];
            }
            tokens.push_back(func);
        }
        else {
            if ((expr[i] == '-' || expr[i] == '+') &&
                (tokens.empty() || tokens.back() == "(" || isSin(tokens.back()) ||
                    tokens.back() == "+" || tokens.back() == "-" || tokens.back() == "*" ||
                    tokens.back() == "/" || tokens.back() == "^")) {
                std::string unary = "";
                unary += expr[i++];
                if (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                    std::string num;
                    while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                        num += expr[i++];
                    }
                    tokens.push_back(unary + num);
                }
                else if (i < expr.length() && expr[i] == '(') {
                    tokens.push_back(unary + "1");
                    tokens.push_back("*");
                }
                else {
                    std::cerr << "Error with operator unary." << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            else {
                std::string op(1, expr[i++]);
                tokens.push_back(op);
            }
        }
    }
    return tokens;
}

std::vector<std::string> infixToRPN(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> opStack;

    for (const auto& token : tokens) {
        if (std::isdigit(token[0]) ||
            ((token.length() > 1) && (token[0] == '-' || token[0] == '+') && std::isdigit(token[1])) ||
            (token[0] == '.' && token.length() > 1 && std::isdigit(token[1]))) {
            output.push_back(token);
        }
        else if (isSin(token)) {
            opStack.push(token);
        }
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty() && opStack.top() == "(") {
                opStack.pop();
            }
            if (!opStack.empty() && isSin(opStack.top())) {
                output.push_back(opStack.top());
                opStack.pop();
            }
        }
        else {
            while (!opStack.empty() &&
                ((getPrecedence(opStack.top()) > getPrecedence(token)) ||
                    (getPrecedence(opStack.top()) == getPrecedence(token) && !isRightAssociative(token))) &&
                opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
    }
    while (!opStack.empty()) {
        if (opStack.top() == "(" || opStack.top() == ")") {
            std::cerr << "Error: unbalanced parentheses." << std::endl;
            exit(EXIT_FAILURE);
        }
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}
double evaluateRPN(const std::vector<std::string>& tokens, bool degree = false) {
    HMODULE hModule_power = LoadLibrary("funcdeg.dll");
    if (hModule_power == nullptr) {
        std::cerr << "Failed to load funcdeg.dll!" << std::endl;
        return 1;
    }

    HMODULE hModule_sin = LoadLibrary("funcsin.dll");
    if (hModule_sin == nullptr) {
        std::cerr << "Failed to load funcsin.dll!" << std::endl;
        FreeLibrary(hModule_power);
        return 1;
    }
    std::stack<double> evalStack;

    for (const auto& token : tokens) {
        if (std::isdigit(token[0]) ||
            ((token.length() > 1) && (token[0] == '-' || token[0] == '+') && std::isdigit(token[1])) ||
            (token[0] == '.' && token.length() > 1 && std::isdigit(token[1]))) {
            evalStack.push(std::stod(token));
        }
        else if (isSin(token)) {
            if (evalStack.empty()) {
                std::cerr << "Error: not enough operands for function" << token << "." << std::endl;
                exit(EXIT_FAILURE);
            }
            double a = evalStack.top(); evalStack.pop();
            if (degree) {
                a = a * M_PI / 180.0;
            }
            if (token == "sin") {
                evalStack.push(calculate_sin(a, hModule_sin));
            }
            else {
                std::cerr << "Error: function not supported" << token << "." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else {
            if (evalStack.size() < 2) {
                std::cerr << "Error: not enough operands for function " << token << "." << std::endl;
                exit(EXIT_FAILURE);
            }
            double b = evalStack.top(); evalStack.pop();
            double a = evalStack.top(); evalStack.pop();
            if (token == "+") {
                evalStack.push(a + b);
            }
            else if (token == "-") {
                evalStack.push(a - b);
            }
            else if (token == "*") {
                evalStack.push(a * b);
            }
            else if (token == "/") {
                if (b == 0) {
                    std::cerr << "Error: divide by 0" << std::endl;
                    exit(EXIT_FAILURE);
                }
                evalStack.push(a / b);
            }
            else if (token == "^") {
                evalStack.push(calculate_power(a, b, hModule_power));
            }
            else {
                std::cerr << "Error: invalid operator" << token << "." << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    if (evalStack.size() != 1) {
        std::cerr << "Error: Invalid expression." << std::endl;
        exit(EXIT_FAILURE);
    }
    FreeLibrary(hModule_power);
    FreeLibrary(hModule_sin);
    return evalStack.top();
}


int main() {
    std::string expression;
    char angleUnitChoice;
    bool degree = false;

    std::cout << "Select the angle unit for trigonometric functions (d: Degrees, r: Radians): ";
    std::cin >> angleUnitChoice;
    std::cin.ignore();
    if (angleUnitChoice == 'd' || angleUnitChoice == 'D') {
        degree = true;
    }
    else if (angleUnitChoice == 'r' || angleUnitChoice == 'R') {
        degree = false;
    }
    else {
        std::cerr << "Invalid selection. Use default Radian." << std::endl;
    }

    std::cout << "Enter expression:";
    std::getline(std::cin, expression);
    std::vector<std::string> tokens = tokenize(expression);
    if (tokens.empty()) {
        std::cerr << "Error: Empty expression." << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<std::string> rpn = infixToRPN(tokens);
    double result = evaluateRPN(rpn, degree);
    std::cout << "Result: " << result << std::endl;
    return 0;
}
