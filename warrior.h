#ifndef WARRIOR_H
#define WARRIOR_H
#include <string>
#include <iostream>

class Warrior
{
public:
    Warrior();
    Warrior(std::string, int, int, int, bool, bool, bool);
    
    //Getters
    std::string getName();
    int getStrength();
    int getLoyalty();
    int getMorale();
    bool isFree();
    bool hasShip();
    bool has_DG();
    int getRow();
    int getColumn();
    
    //Setters
    void setName(std::string);
    void setStrength(int);
    void setLoyalty(int);
    void setMorale(int);
    void setFree(bool);
    void setShip(bool);
    void set_DG(bool);
    void setrow(int);
    void setcolumn(int);
    void setLocation(int, int);
    
private:
    std::string name;
    int warriorStrength;
    int warriorLoyalty;
    int warriorMorale;
    int r;
    int c;
    bool warriorFree;
    bool shipAvail;
    bool dragonglass_avail;
};
#endif // WARRIORS_H
