// sin.cpp
#include <cmath>
#include "pch.h"
#include "sin.h"

const char* SinExpPlugin::get_name() const {
    return "sin"; // Tên plugin
}

int SinExpPlugin::get_num_params() const {
    return 1; // Plugin này chỉ cần một tham số cho hàm sin
}

double SinExpPlugin::execute(double angle, double /*unused*/) {
    return std::sin(angle); // Tính giá trị sin của tham số
}

// Hàm xuất DLL để tạo đối tượng plugin
extern "C" __declspec(dllexport) IPlugin * create_plugin() {
    return new SinExpPlugin();
}

// Hàm xuất DLL để hủy đối tượng plugin
extern "C" __declspec(dllexport) void destroy_plugin(IPlugin * plugin) {
    delete plugin;
}
