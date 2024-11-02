#include "../Calculator/Calculator.h"

Calculator::Calculator::Calculator() {
    // Load necessary plugins
    importer.loadPlugin("F:/C++/Calculator/plugins/sin.dll"); // Load sin plugin
    importer.loadPlugin("F:/C++/Calculator/plugins/pow.dll"); // Load pow plugin

    unaryFunctions = {
        {"sin", Functions([this](double x, double) { return importer.executePlugin("sin", x); })},
    };

    binaryFunctions = {
        {"+", Functions([](double x, double y) { return x + y; }, TokenType::Operator, 2)},
        {"-", Functions([](double x, double y) { return x - y; }, TokenType::Operator, 2)},
        {"*", Functions([](double x, double y) { return x * y; }, TokenType::Operator, 3)},
        {"/", Functions([](double x, double y) { 
            if (y == 0) throw std::runtime_error("Division by zero.");
            return x / y;}, TokenType::Operator, 3)},
        {"^", Functions([this](double x, double y) { return importer.executePlugin("power", x, y); }, TokenType::Operator, 4, false)},
    };
}

bool Calculator::Calculator::isNumber(const std::string& token) {
    return !token.empty() && (isdigit(token[0]) || token[0] == '.');
}

int Calculator::Calculator::getPrecedence(const std::string& op) {
    if (binaryFunctions.count(op)) {
        return binaryFunctions[op].precedence;
    }
    return 0;
}

bool Calculator::Calculator::isLeftAssociative(const std::string& op) {
    if (binaryFunctions.count(op)) {
        return binaryFunctions[op].leftAssociative;
    }
    return true;
}

std::queue<std::string> Calculator::Calculator::toRPN(const std::string& expression) {
    std::queue<std::string> outputQueue;
    std::stack<std::string> operatorStack;
    std::string token;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isspace(c)) continue;

        // Parse numbers
        if (isdigit(c) || c == '.') {
            token = c;
            while (i + 1 < expression.length() && (isdigit(expression[i + 1]) || expression[i + 1] == '.')) {
                token += expression[++i];
            }
            outputQueue.push(token);
        }
        // Parse functions and variables
        else if (isalpha(c)) {
            std::string funcName;
            while (i < expression.length() && isalpha(expression[i])) {
                funcName += expression[i++];
            }
            --i;  // Step back after loop

            if (unaryFunctions.count(funcName)) {
                operatorStack.push(funcName);
            }
            else if (constants.count(funcName)) {
                outputQueue.push(std::to_string(constants[funcName]));
            }
        }
        // Parse operators
        else if (binaryFunctions.count(std::string(1, c))) {
            std::string op(1, c);

            while (!operatorStack.empty()) {
                std::string topOp = operatorStack.top();

                if ((binaryFunctions[op].precedence < getPrecedence(topOp)) ||
                    (binaryFunctions[op].precedence == getPrecedence(topOp) && isLeftAssociative(op))) {
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                }
                else {
                    break;
                }
            }
            operatorStack.push(op);
        }
        // Handle parentheses
        else if (c == '(') {
            operatorStack.push("(");
        }
        else if (c == ')') {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty()) operatorStack.pop();

            // Pop function if there is one right before the '('
            if (!operatorStack.empty() && unaryFunctions.count(operatorStack.top())) {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
        }
    }

    while (!operatorStack.empty()) {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}

Calculator::Node* Calculator::Calculator::parseRPN(std::queue<std::string>& rpnQueue) {
    std::stack<Node*> stack;

    while (!rpnQueue.empty()) {
        std::string token = rpnQueue.front();
        rpnQueue.pop();

        if (isNumber(token)) {
            stack.push(new Node(std::stod(token)));
        }
        else if (binaryFunctions.count(token)) {
            if (stack.size() < 2) throw std::runtime_error("Insufficient operands for binary operator.");

            Node* node = new Node(binaryFunctions[token], false);
            node->right = stack.top(); stack.pop();
            node->left = stack.top(); stack.pop();
            stack.push(node);
        }
        else if (unaryFunctions.count(token)) {
            if (stack.empty()) throw std::runtime_error("Insufficient operands for unary function.");

            Node* node = new Node(unaryFunctions[token]);
            node->left = stack.top(); stack.pop();
            stack.push(node);
        }
    }
    if (stack.size() != 1) throw std::runtime_error("Invalid RPN expression.");

    return stack.top();
}

double Calculator::Calculator::evaluate(Node* node) {
    if (node->isFunc) {
        if (node->isUnary) {
            return node->func.eval(evaluate(node->left), 0);
        }
        else {
            return node->func.eval(evaluate(node->left), evaluate(node->right));
        }
    }
    return node->value;
    return node->value;
}

double Calculator::Calculator::calculate(const std::string& expression) {
    std::queue<std::string> rpnQueue = toRPN(expression);
    Node* rootNode = parseRPN(rpnQueue);
    double result = evaluate(rootNode);

    // Clean up memory (you might want to implement a more robust solution)
    delete rootNode;

    return result;
}
