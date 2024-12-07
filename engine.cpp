#include "wrapper.h"
#include "engine.h"
#include <iostream>
#include <map>

bool Engine::register_command(Wrapper* wrapper, const std::string& command_name) {
    if (wrappers.find(command_name) != wrappers.end()) {
        std::cerr << "Command is already registered: " << command_name << std::endl;
        return false;
    }
    wrappers[command_name] = wrapper;
    return true;
}

std::string Engine::execute(const std::string& command_name, const ArgsMap& args_map) {
    if (wrappers.find(command_name) != wrappers.end()) {
        return wrappers[command_name]->execute(args_map);
    } else {
        std::cerr << "Command was not registered: " << command_name << std::endl;
        return "";
    }
}