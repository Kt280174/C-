#pragma once
#ifndef POW_H
#define POW_H
#include <cmath>
#include <windows.h>
#include "../Calculator/IPlugin.h"

class PowerExpPlugin : public IPlugin {
public:
    const char* get_name() const override;

    int get_num_params() const override;

    double execute(double base, double exponent) override;
};

extern "C" __declspec(dllexport) IPlugin * create_plugin();

extern "C" __declspec(dllexport) void destroy_plugin(IPlugin * plugin);
#endif