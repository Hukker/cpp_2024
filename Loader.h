#pragma once
#include <string>
#include <windows.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <filesystem>

class Loader {
private:
    std::vector<HMODULE> lodaded_modules;
    std::unordered_map<std::string, FARPROC> function_map;
public:
    Loader(const std::string& directory);
    ~Loader();

    double method_function(const std::string& function_name, double arg);
    double method_function(const std::string& function_name, double arg1, double arg2);
};