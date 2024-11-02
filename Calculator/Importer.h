// Importer.h
#ifndef IMPORTER_H
#define IMPORTER_H

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <windows.h>
#include "../Calculator/IPlugin.h"
#include "../Calculator/TokenType.h"
namespace Calculator{
    class Importer {
    public:
        Importer();
        void loadPlugin(const std::string& path);
        double executePlugin(const std::string& name, double param1, double param2 = 0);

    private:
        std::unordered_map<IPlugin*, HMODULE> plugins; // Danh sách plugin và handle DLL
    };
}

#endif // IMPORTER_H
