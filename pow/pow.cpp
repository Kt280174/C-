#include "pow.h"
#include "pch.h"
#include <cmath>
// pow.cpp
#include "pow.h"

const char* PowerExpPlugin::get_name() const {
    return "power"; // Tên plugin
}

int PowerExpPlugin::get_num_params() const {
    return 2; // Plugin này yêu cầu hai tham số: cơ số và số mũ
}

double PowerExpPlugin::execute(double base, double exponent) {
    return std::pow(base, exponent); // Tính lũy thừa
}

// Hàm xuất DLL để tạo đối tượng plugin
extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new PowerExpPlugin();
}

// Hàm xuất DLL để hủy đối tượng plugin
extern "C" __declspec(dllexport) void destroy_plugin(IPlugin * plugin) {
    delete plugin;
}

