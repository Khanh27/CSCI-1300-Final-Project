#ifndef CITY_H
#define CITY_H
#include <string>

class City
{
public:
    City();
    City(std::string, std::string, int,int,int);
    
    std::string getLetter(); 
    std::string getName();
    int getMoney();
    int getArmy();
    int getPoints();
    void setLetter(std::string);
    void setName(std::string);
    void setMoney(int);
    void setArmy(int);
    void setPoints(int);
    
    
    
private:
    std::string letter;
    std::string name;
    int money;
    int army;
    int points;
};

#endif // CITY_H