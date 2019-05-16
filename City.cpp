#include "City.h"
#include <iostream>
#include <string>

City::City(){
     letter = "";
     name = "";
     money = 0;
     army = 0;
     points = 0;
};

City::City(std::string l, std::string n, int m, int a, int p)
{
  letter = l;
  name = n;
  money = m;
  army = a;
  points = p;
};
    
std::string City::getLetter() 
{
    return letter;
};

std::string City::getName()
{
    return name;
};

int City::getMoney()
{
    return money;
};
int City::getArmy()
{
  return army;  
};

int City::getPoints()
{
    return points;
};
void City::setLetter(std::string l)
{
    letter = l;
};
void City::setName(std::string n){
    name = n;
};
void City::setMoney(int m){
    money = m;
};
void City::setArmy(int a){
    army = a;
};
void City::setPoints(int p)
{
    points = p;
};