#pragma once
#ifndef SIN_H
#define SIN_H
#include <cmath>
#include <windows.h>
#include "../Calculator/IPlugin.h"

class SinExpPlugin : public IPlugin {
public:
    const char* get_name() const override;

    int get_num_params() const override;

    double execute(double base, double exponent = 0) override;
};

extern "C" __declspec(dllexport) IPlugin * create_plugin();

extern "C" __declspec(dllexport) void destroy_plugin(IPlugin * plugin);
#endif