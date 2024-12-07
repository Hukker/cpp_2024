#pragma once
#include "wrapper.h"
#include <map>

class Engine {
private:
    std::map<std::string, Wrapper*> wrappers;
public:
    bool register_command(Wrapper* wrapper, const std::string& command_name);
    std::string execute(const std::string& command_name, const ArgsMap& args_map);
};
