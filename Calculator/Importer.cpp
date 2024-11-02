// Importer.cpp
#include "../Calculator/Importer.h"
#include <iostream>

Calculator::Importer::Importer() = default;

void Calculator::Importer::loadPlugin(const std::string& path) {
    HMODULE hModule = LoadLibraryA(path.c_str());
    if (!hModule) {
        throw std::runtime_error("Could not load the DLL: " + path);
    }

    using CreatePluginFunc = IPlugin * (*)();
    CreatePluginFunc createPlugin = reinterpret_cast<CreatePluginFunc>(GetProcAddress(hModule, "create_plugin"));
    if (!createPlugin) {
        FreeLibrary(hModule);
        throw std::runtime_error("Could not find create_plugin function in " + path);
    }

    IPlugin* plugin = createPlugin();
    plugins[plugin] = hModule;
}

double Calculator::Importer::executePlugin(const std::string& name, double param1, double param2) {
    for (const auto& plugin : plugins) {
        if (plugin.first->get_name() == name) {
            if (plugin.first->get_num_params() == 1) {
                return plugin.first->execute(param1);
            }
            else if (plugin.first->get_num_params() == 2) {
                return plugin.first->execute(param1, param2);
            }
        }
    }
    throw std::runtime_error("Plugin not found: " + name);
}
