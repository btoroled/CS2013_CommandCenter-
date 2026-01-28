//
// Created by Benjamin Toro Leddihn on 24/01/26.
//

#ifndef CS2013_COMMANDCENTER__ENTITY_H
#define CS2013_COMMANDCENTER__ENTITY_H

#include <map>
#include <string>
#include <sstream>

class Entity {
private:
    int x_ = 0;
    int y_ = 0;

    int hp_ = 10;
    int max_hp_ = 10;

    std::string name_ = "Unknown";
    int level_ = 1;
    int base_ac_ = 10;

    struct Weapon { std::string name; int dmg = 0; };
    struct Armor  { std::string name; int ac_bonus = 0; };

    bool has_weapon_ = false;
    Weapon weapon_{};

    bool has_armor_ = false;
    Armor armor_{};

    std::map<std::string, int> stats_;

    struct Snapshot {
        int x = 0, y = 0, hp = 0, max_hp = 0, level = 1, base_ac = 10;
        bool has_weapon = false;
        Weapon weapon{};
        bool has_armor = false;
        Armor armor{};
        std::map<std::string, int> stats;
    } start_;

public:
    Entity();
    Entity(std::string name, int level, int max_hp);

    // getters
    int x() const;
    int y() const;
    int hp() const;
    int maxHp() const;
    int level() const;
    int ac() const;

    std::string snapshot() const;

    void moveTo(int x, int y);
    void heal(int amount);
    void damage(int amount);

    void equipWeapon(std::string name, int dmg);
    void equipArmor(std::string name, int ac_bonus);

    void startCombatSnapshot();
    void reset();

    void printStatus(std::ostream& os) const;
};

#endif //CS2013_COMMANDCENTER__ENTITY_H