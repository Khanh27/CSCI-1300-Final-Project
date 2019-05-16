#include "hero.h"
#include <string>
#include <iostream>

Hero::Hero()
{
    alive = true;
     r = 0; // row
     c = 0; // collum
     numWarriors = 0;
     name = "";
     money = 0;
     influence = 0;
     army = 0;
    for(int i = 0; i < 4; i++)
    {
        warriors[i] = "";
    }
     ship = false;
     points = 0;
};


Hero::Hero(std::string n, int m, int influ, int a,std::string w[4], int row, int col, bool s)
{
    name = n;
    numWarriors = 0;
    for(int i = 0; i < 4; i++)
    {
        warriors[i] = w[i];
        if (w[i] != "")
        {
            numWarriors++;
        }
    }
    money = m;
    influence = influ;
    army = a;
    r = row;
    c = col;
    ship = s;
    //p = points;
};
    
    //SETTERS
    void Hero::setName(std::string n)
    {
        name = n;
    };
    void Hero::setMoney(int m)
    {
        money = m;
    };
    void Hero::setInfluence(int influ)
    {
        influence = influ;
    };
    void Hero::addWarrior(std::string w)
    {
        for(int i = 0; i < 4; i++)
        {
            if(warriors[i] == "")
            {
                warriors[i] = w;
                numWarriors++;
                return;
            }
        }
    };
    void Hero::removeWarrior(std::string w)
    
    {
        for(int i = 0; i < 4; i++)
        {
            if(warriors[i] == w)
            {
                warriors[i] = "";
                numWarriors--;
                return;
            }
        }
    };
    void Hero::setHeroLocation(int row, int col)
    {
        r = row;
        c = col;
    };
    void Hero::setShip(bool s)
    {
        ship = s;
    };
    void Hero::setArmysize(int a)
    {
        army = a;
    };
    
    void Hero::setHeropoints(int p)
    {
        points = p;
    }

    //GETTERS
    std::string Hero::getName()
    {
        return name;
    };
    int Hero::getMoney()
    {
        return money;
    };
    int Hero::getInfluence()
    {
        return influence;
    };
    std::string Hero::getWarriorByIndex(int i)
    {
        return warriors[i];
    };
    int Hero::getRow()
    {
        return r;
    }
    int Hero::getCol()
    {
        return c;
    };
    int Hero::getArmy()
    {
        return army;
    };
    int Hero::getHeropoints()
    {
        return points;
    };
    bool Hero::checkShip()
    {
        return ship;
    }
    
    int Hero::getWarriorsize() //problematic
    {
       return numWarriors;
    }
    
   bool Hero::checkAlive()
   {
       return alive;
   }
   
   void Hero::setAlive(bool life)
   {
       alive = life;
   }
    
    