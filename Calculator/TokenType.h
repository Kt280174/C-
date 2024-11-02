#pragma once
#pragma once
#ifndef TOKENTYPE_H
#define TOKENTYPE_H
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


#pragma once

namespace Calculator {

    enum class TokenType {
        Operator,
        Constant,
        Function,
        LeftParen,
        RightParen,
        Unknown
    };

} // namespace Calculator

#endif // TOKENTYPE_H

