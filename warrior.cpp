#include "warrior.h"
#include <string>
#include <iostream>


Warrior::Warrior()
{
     name = "";
     warriorStrength = 0;
     warriorLoyalty = 0;
     warriorMorale = 0;
     warriorFree = false;
     shipAvail = false; 
     dragonglass_avail = false;
     r = 0;
     c = 0;
};

Warrior::Warrior(std::string n, int str, int lyt, int mrl, bool wFree, bool S_avail, bool DG_avail)
{
    name = n;
    warriorStrength = str;
    warriorLoyalty = lyt;
    warriorMorale = mrl;
    warriorFree = wFree;
    shipAvail = S_avail; 
    dragonglass_avail = DG_avail;
};
    
    //Getters
    std::string Warrior::getName()
    {
        return name;
    };
    int Warrior::getStrength()
    {
        return warriorStrength;
    };
    int Warrior::getLoyalty()
    {
        return warriorLoyalty;
    };
    int Warrior::getMorale()
    {
        return warriorMorale;
    };
    bool Warrior::isFree()
    {
        return warriorFree;
    };
    bool Warrior::hasShip()
    {
        return shipAvail;
    };
    bool Warrior::has_DG()
    {
        return dragonglass_avail;
    };
    int Warrior::getRow()
    {
        return r;
    }
    int Warrior::getColumn()
    {
        return c;
    }
    
    //Setters
    void Warrior::setName(std::string n)
    {
        name = n;
    };
    void Warrior::setStrength(int str)
    {
        warriorStrength = str;
    };
    void Warrior::setLoyalty(int lyt)
    {
        warriorLoyalty = lyt;
    };
    void Warrior::setMorale(int mrl)
    {
        warriorMorale = mrl;
    };
    void Warrior::setFree(bool wFree)
    {
        warriorFree = wFree;
    };
    void Warrior::setShip(bool S_avail)
    {
        shipAvail = S_avail;
    };
    void Warrior::set_DG(bool DG_avail)
    {
        dragonglass_avail = DG_avail;
    };
    
    void Warrior::setLocation(int row,int column)
    {
        r = row;
        c = column;
    };
