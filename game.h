#ifndef GAME_H
#define GAME_H
#include <string>
#include "City.h"
#include "hero.h"
#include "warrior.h"



class Game
{
    public:
    Game();
    
    //initializers
    void createBoard(std::string);
    void setDG_location(); 
    void readWarrior(std::string);  // read the warrior file
    void readHero(std::string); // read the hero file
    void readCity(std::string); // read the city file
    void setpointhero();
    void setStatusforCustomHero(std::string);
    void Displaytext();
    void printMap(int, int);
    void moving(int, int);
    void restChoice(int);
    void MainChoices();
    void consultGod(int);
    void encounters(int);
    void ComputerMove();
    void randomMovement();
    void battleChoices();
    int check_encounter(int);
    void choosewarriors(int, int);
    void freeWarriorMoves();
    int getWarriorMorale(int);
    void OutOfEZ(int, int);
    void PlayerStatsReport();
    void endOfStannis();
    void endGame();
    
    //getters
    int getHero(std::string); // get specific hero
    int getWarrior(std::string); // get specific warrior
    int getCity(std::string); // get specific city
    bool isDG(int, int); // check if dragon glass exist
    //bool isWater(int, int);
    
    //setters
    
    private:
    std::string board[25][16];
    int dG_location[5][2];
    Hero heroes[7];
    City cities[26];
    Warrior warriors[14];
    int player;
    bool gameEnd;
    int turnCount;
};

#endif