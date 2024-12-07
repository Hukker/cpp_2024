#include "wrapper.h"
#include <vector>
#include <functional>
#include <string>
#include <iostream>
#include <algorithm>

using InputArgs = std::vector<std::pair<std::string, std::string>>;
using ArgsMap = std::vector<std::pair<std::string, std::string>>;

std::string Wrapper::execute(const ArgsMap& args_map) {
    if (args_map.size() != argsTitles.size()) {
        std::cerr << "Wrong number of arguments." << std::endl;
        return "";
    }
    std::vector<std::string> argsFunc;
    for (const auto& title : argsTitles) {
        auto it = std::find_if(args_map.begin(), args_map.end(), [&title](const std::pair<std::string, std::string>& pair) {
            return pair.first == title;
        });
        if (it == args_map.end()) {
            std::cerr << "Missing argument: " << title << std::endl;
            return "";
        }
        argsFunc.push_back(it->second);
    }
    return function(argsFunc);
}