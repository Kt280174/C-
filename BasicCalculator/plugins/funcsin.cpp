// math_functions.cpp
#include <cmath>

extern "C" {
    __declspec(dllexport) double sin_function(double x) {
        return sin(x);
    }
}
