#ifndef HERO_H
#define HERO_H
#include "warrior.h"
#include <string>
#include <array>
#include <iostream>

class Hero
{
public:
    Hero();
    Hero(std::string, int, int, int,std::string[4], int, int, bool);
    
    //SETTERS
    void setName(std::string);
    void setMoney(int);
    void setInfluence(int);
    void addWarrior(std::string);
    void removeWarrior(std::string);
    void setHeroLocation(int, int);
    void setShip(bool);
    void setArmysize(int);
    void setHeropoints(int);
    //void setLocation(int, int);
    void setAlive(bool);
    
    //GETTERS
    std::string getName();
    bool checkShip();
    int getMoney();
    int getInfluence();
   // std::string getWarriorbyIndex();
    int getRow();
    int getCol();
    int getArmy();
    int getHeropoints();
    //int setLocation(;)
    int getWarriorsize();
    std::string getWarriorByIndex(int);
    //int getWarriorMorale(int);
    bool checkAlive();
    

private:
    int r; // row
    int c; // collum
    std::string name;
    int money;
    int influence;
    int army;
    std::string warriors[4];
    bool ship;
    int points;
    int numWarriors;
    bool alive;
};


#endif // HEROES_H