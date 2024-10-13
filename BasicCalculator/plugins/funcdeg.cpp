// plugins/math_power.cpp
#include <cmath>

extern "C" {
    __declspec(dllexport) double power_function(double base, double exponent) {
        return pow(base, exponent);
    }
}
