//
// Created by Benjamin Toro Leddihn on 24/01/26.
//

#include "../includes/Entity.h"

#include <utility>

Entity::Entity() {
    stats_["STR"] = 10;
    stats_["DEX"] = 10;
    stats_["CON"] = 10;

    startCombatSnapshot();
}

Entity::Entity(std::string name, int level, int max_hp)
    : hp_(max_hp), max_hp_(max_hp), name_(std::move(name)), level_(level) {
    stats_["STR"] = 12;
    stats_["DEX"] = 14;
    stats_["CON"] = 11;

    startCombatSnapshot();
}

int Entity::x() const { return x_; }
int Entity::y() const { return y_; }
int Entity::hp() const { return hp_; }
int Entity::maxHp() const { return max_hp_; }
int Entity::level() const { return level_; }

int Entity::ac() const {
    int bonus = 0;
    if (has_armor_) bonus += armor_.ac_bonus;
    return base_ac_ + bonus;
}

std::string Entity::snapshot() const {
    std::ostringstream oss;
    oss << "{name=" << name_
        << ", lvl=" << level_
        << ", hp=" << hp_ << "/" << max_hp_
        << ", ac=" << ac()
        << ", pos=(" << x_ << "," << y_ << ")";
    if (has_weapon_) oss << ", weapon=" << weapon_.name << "+" << weapon_.dmg;
    if (has_armor_)  oss << ", armor="  << armor_.name  << "+" << armor_.ac_bonus;
    oss << "}";
    return oss.str();
}

void Entity::moveTo(int x, int y) { x_ = x; y_ = y; }

void Entity::heal(int amount) {
    if (amount <= 0) return;
    hp_ += amount;
    if (hp_ > max_hp_) hp_ = max_hp_;
}

void Entity::damage(int amount) {
    if (amount <= 0) return;
    hp_ -= amount;
    if (hp_ < 0) hp_ = 0;
}

void Entity::equipWeapon(std::string name, int dmg) {
    has_weapon_ = true;
    weapon_ = Weapon{std::move(name), dmg};
}

void Entity::equipArmor(std::string name, int ac_bonus) {
    has_armor_ = true;
    armor_ = Armor{std::move(name), ac_bonus};
}

void Entity::startCombatSnapshot() {
    start_.x = x_; start_.y = y_;
    start_.hp = hp_; start_.max_hp = max_hp_;
    start_.level = level_; start_.base_ac = base_ac_;

    start_.has_weapon = has_weapon_;
    start_.weapon = weapon_;

    start_.has_armor = has_armor_;
    start_.armor = armor_;

    start_.stats = stats_;
}

void Entity::reset() {
    x_ = start_.x; y_ = start_.y;
    hp_ = start_.hp; max_hp_ = start_.max_hp;
    level_ = start_.level; base_ac_ = start_.base_ac;

    has_weapon_ = start_.has_weapon;
    weapon_ = start_.weapon;

    has_armor_ = start_.has_armor;
    armor_ = start_.armor;

    stats_ = start_.stats;
}

void Entity::printStatus(std::ostream& os) const {
    os << snapshot() << "\n";
    os << "  stats:";
    for (auto it = stats_.begin(); it != stats_.end(); ++it) {
        os << " " << it->first << "=" << it->second;
    }
    os << "\n";
}
