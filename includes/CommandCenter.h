//
// Created by Benjamin Toro Leddihn on 27/01/26.
//

#ifndef CS2013_COMMANDCENTER__COMMANDCENTER_H
#define CS2013_COMMANDCENTER__COMMANDCENTER_H

#include <functional>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <iostream>

#include "Entity.h"

class CommandCenter {
public:
    using Command = std::function<void(const std::list<std::string>&)>;

private:
    Entity& entity_;
    std::map<std::string, Command> commands_;
    std::list<std::string> history_;

    using MacroSteps = std::list<std::pair<std::string, std::list<std::string>>>;
    std::map<std::string, MacroSteps> macros_;

public:
    explicit CommandCenter(Entity& e);

    Entity& entity();

    void registerCommand(const std::string& name, Command cmd);
    bool removeCommand(const std::string& name);

    bool execute(const std::string& name, const std::list<std::string>& args);

    void printHistory(std::ostream& os);

    void registerMacro(const std::string& name, const MacroSteps& steps);
    bool executeMacro(const std::string& name);
};

#endif //CS2013_COMMANDCENTER__COMMANDCENTER_H