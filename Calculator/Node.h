#pragma once
#ifndef NODE_H
#define NODE_H
#define _USE_MATH_DEFINES
#include "../Calculator/function.h"
#include<cmath>
#include <string>
namespace Calculator {
    class Node {
    public:
        double value;
        Functions func;
        bool isFunc;
        bool isUnary;
        Node* left;
        Node* right;
        std::string name;

        // Constructors for constant and function nodes
        Node(double val) : value(val), isFunc(false), isUnary(false), left(nullptr), right(nullptr) {}
        Node(Functions func, bool unary = true) : func(func), isFunc(true), isUnary(unary), left(nullptr), right(nullptr) {}
    };
}

#endif // NODE_H

