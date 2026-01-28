//
// Created by Benjamin Toro Leddihn on 27/01/26.
//

#include "../includes/CommandCenter.h"

#include <sstream>

static std::string joinArgs(const std::list<std::string>& args) {
    std::ostringstream oss;
    bool first = true;
    for (std::list<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        if (!first) oss << ' ';
        oss << *it;
        first = false;
    }
    return oss.str();
}

CommandCenter::CommandCenter(Entity& e) : entity_(e) {}
Entity& CommandCenter::entity() { return entity_; }

void CommandCenter::registerCommand(const std::string& name, Command cmd) {
    commands_[name] = std::move(cmd);
}

bool CommandCenter::removeCommand(const std::string& name) {
    std::map<std::string, Command>::iterator it = commands_.find(name);
    if (it == commands_.end()) {
        std::cerr << "[ERROR] removeCommand: '" << name << "' no existe\n";
        return false;
    }
    commands_.erase(it);
    return true;
}

bool CommandCenter::execute(const std::string& name, const std::list<std::string>& args) {
    std::map<std::string, Command>::iterator it = commands_.find(name);
    if (it == commands_.end()) {
        std::cerr << "[ERROR] execute: comando '" << name << "' no registrado\n";
        return false;
    }

    const std::string before = entity_.snapshot();
    it->second(args);
    const std::string after = entity_.snapshot();

    std::ostringstream oss;
    oss << name;
    if (!args.empty()) oss << " " << joinArgs(args);
    oss << " | " << before << " -> " << after;
    history_.push_back(oss.str());
    return true;
}

void CommandCenter::printHistory(std::ostream& os) {
    os << "\n=== HISTORIAL (" << history_.size() << ") ===\n";
    std::list<std::string>::iterator it = history_.begin();
    for (; it != history_.end(); ++it) os << *it << "\n";
}

void CommandCenter::registerMacro(const std::string& name, const MacroSteps& steps) {
    macros_[name] = steps;
}

bool CommandCenter::executeMacro(const std::string& name) {
    std::map<std::string, MacroSteps>::iterator it = macros_.find(name);
    if (it == macros_.end()) {
        std::cerr << "[ERROR] executeMacro: macro '" << name << "' no registrado\n";
        return false;
    }

    const MacroSteps& steps = it->second;
    for (MacroSteps::const_iterator sit = steps.begin(); sit != steps.end(); ++sit) {
        const std::string& cmdName = sit->first;
        const std::list<std::string>& cmdArgs = sit->second;

        if (!execute(cmdName, cmdArgs)) {
            std::cerr << "[ERROR] macro '" << name << "' detenida en paso: " << cmdName << "\n";
            return false;
        }
    }
    return true;
}