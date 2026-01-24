//
// Created by Benjamin Toro Leddihn on 24/01/26.
//

#ifndef CS2013_COMMANDCENTER__ENTITY_H
#define CS2013_COMMANDCENTER__ENTITY_H
#include <list>
#include<vector>
#include<map>

class Entity {
    int health;
    std::string name;
    int level;
    int AC;
    std::string raza;
    std::map<std::string,int> stats;

    template<typename T>
    std::vector<T> inventario;
    public:
    Entity(){};
};


#endif //CS2013_COMMANDCENTER__ENTITY_H