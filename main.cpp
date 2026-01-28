#include <list>
#include <string>

#include "includes/Entity.h"
#include "includes/CommandCenter.h"

static bool parseIntStrict(const std::string& s, int& out) {
    try {
        size_t idx = 0;
        int v = std::stoi(s, &idx);
        if (idx != s.size()) return false;
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}

static void printArgError(const std::string& cmd, const std::string& msg) {
    std::cerr << "[ERROR] " << cmd << ": " << msg << "\n";
}

static void cmd_move(Entity& e, const std::list<std::string>& args) {
    if (args.size() != 2) {
        printArgError("move", "se esperan 2 args: move x y");
        return;
    }
    int x = 0, y = 0;
    std::list<std::string>::const_iterator it = args.begin();
    if (!parseIntStrict(*it, x)) { printArgError("move", "x no es entero válido"); return; }
    ++it;
    if (!parseIntStrict(*it, y)) { printArgError("move", "y no es entero válido"); return; }
    e.moveTo(x, y);
}

class RageStrike {
private:
    Entity& e_;
    int uses_ = 0;
    int max_uses_ = 3;

public:
    explicit RageStrike(Entity& e, int max_uses = 3) : e_(e), max_uses_(max_uses) {}

    void operator()(const std::list<std::string>& args) {
        if (args.size() != 1) { printArgError("rage_damage", "se espera 1 arg: rage_damage n"); return; }
        if (uses_ >= max_uses_) { printArgError("rage_damage", "sin usos restantes"); return; }

        int n = 0;
        if (!parseIntStrict(*args.begin(), n) || n <= 0) { printArgError("rage_damage", "n debe ser entero positivo"); return; }

        int bonus = uses_ * 2;
        ++uses_;
        e_.damage(n + bonus);
    }
};

int main() {
    Entity hero("Elbull", 3, 24);
    CommandCenter cc(hero);

    cc.registerCommand("move", [&](const std::list<std::string>& args) {
        cmd_move(cc.entity(), args);
    });

    cc.registerCommand("heal", [&](const std::list<std::string>& args) {
        if (args.size() != 1) { printArgError("heal", "se espera 1 arg: heal n"); return; }
        int n = 0;
        if (!parseIntStrict(*args.begin(), n) || n <= 0) { printArgError("heal", "n debe ser entero positivo"); return; }
        cc.entity().heal(n);
    });

    cc.registerCommand("status", [&](const std::list<std::string>&) {
        cc.entity().printStatus(std::cout);
    });

    cc.registerCommand("reset", [&](const std::list<std::string>& args) {
        if (!args.empty()) { printArgError("reset", "no recibe argumentos"); return; }
        cc.entity().reset();
    });

    RageStrike rage(hero, 3);
    cc.registerCommand("rage_damage", rage);

    cc.registerCommand("equip_weapon", [&](const std::list<std::string>& args) {
        if (args.size() != 2) { printArgError("equip_weapon", "equip_weapon <name> <dmg>"); return; }
        std::list<std::string>::const_iterator it = args.begin();
        std::string wname = *it; ++it;
        int dmg = 0;
        if (!parseIntStrict(*it, dmg) || dmg < 0) { printArgError("equip_weapon", "dmg debe ser entero >= 0"); return; }
        cc.entity().equipWeapon(wname, dmg);
    });

    cc.registerCommand("equip_armor", [&](const std::list<std::string>& args) {
        if (args.size() != 2) { printArgError("equip_armor", "equip_armor <name> <ac_bonus>"); return; }
        std::list<std::string>::const_iterator it = args.begin();
        std::string aname = *it; ++it;
        int bonus = 0;
        if (!parseIntStrict(*it, bonus) || bonus < 0) { printArgError("equip_armor", "ac_bonus debe ser entero >= 0"); return; }
        cc.entity().equipArmor(aname, bonus);
    });

    hero.startCombatSnapshot();

    cc.execute("move", {"10", "20"});
    cc.execute("heal", {"3"});
    cc.execute("rage_damage", {"4"});
    cc.execute("equip_weapon", {"Dagger", "3"});
    cc.execute("equip_armor", {"Leather", "2"});
    cc.execute("status", {});

    using Steps = std::list<std::pair<std::string, std::list<std::string>>>;
    Steps opening = {
        {"move", {"1", "1"}},
        {"rage_damage", {"5"}},
        {"status", {}}
    };
    cc.registerMacro("opening_combo", opening);
    cc.executeMacro("opening_combo");

    cc.printHistory(std::cout);
}