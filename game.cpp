#include <string>
#include "City.h"
#include "hero.h"
#include <cstdlib>
#include "warrior.h"
#include "game.h"
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

// CS1300 Spring 2019
// Author: Tri Bui
// Recitation: 208
// Cloud9 Workspace Editor Link: https://ide.c9.io/applepineapple/csci1300
// Project 3

int split(std::string str, char delim, std::string words[])
{
    if (delim == ' '&& str == "") // if delimiter has space, and string contains nothing
    {
        return 0;
    }
    std::string token; // string will contain the split string
    int it = 0;
    size_t pos = 0; 
  while ((pos = str.find(delim)) < str.length() -1 )
  { 
    token = str.substr(0, pos); // add the string being split into token
    
    if (pos != 0)
    {
        words[it] = token; // add token to the element of the array. 
        it++;
    }
    
    str.erase(0, pos + 1); // exclude out the 
  }
  words[it] = str;
  
  return (it + 1);
}

void Game::Displaytext() // display text
{
    string choices;
    string username;
    int herochoice;
    cout << "Welcome to the GAME OF THRONES!" << endl;
    cout << "State your name, your grace" << endl;
    cin >> username;
    
    cout << "Would you like to choose a hero " << username << ", Your grace? (Y/N)" << endl;
    //hooblah
    while(cin >> choices)
    {
        if(choices == "Y" ^ choices == "y") // if choice is Y or y then print this statement out
        {
            cout << "Please choose from the following heroes: " << endl;
            cout << "1. Cersei Lannister" << endl;
            cout << "2. Stannis Baratheon" << endl;
            cout << "3. Jon Snow" << endl; 
            cout << "4. Sansa Stark" << endl;
            cout << "5. Euron Greyjoy" << endl;
            cout << "6. Daenerys Targaryen" << endl;
            
            cin >> herochoice;
            switch (herochoice)
            {
                case 1: getHero("Cersei Lannister");
                break;
                
                case 2: getHero("Stannis Baratheon");
                break;
                
                case 3: getHero("Jon Snow");
                break;
                
                case 4: getHero("Sansa Stark");
                break;
                
                case 5: getHero("Euron Greyjoy");
                break;
                
                case 6: getHero("Daenerys Targaryen");
                break;
            }
            player = herochoice - 1;
            break;
        }
        else if (choices == "N" ^ choices == "n") // if choice N or n, then print this statement out
        {
            cout << "Your Grace " << username <<  ", you have chosen to go on this journey alone. Please provide starting values for your adventure." << endl;
            setStatusforCustomHero(username); // function for custom hero
            player = 6;
            break;
        }
        else
        {
            cout << "Please pick a command" << endl;
        }
    }
}


Game::Game()
{
     turnCount = 0;
    createBoard("mapGOT.txt");
    setDG_location();
    readWarrior("warrior.txt");
    readHero("heroGOT.txt");
    readCity("cityGOT.txt");
    setpointhero();
    
    //NOT REAL CODE ITS A CHEAT
    //heroes[0].setHeroLocation(10,5);
    //heroes[3].setHeroLocation(11,5);
    //heroes[5].setHeroLocation(10,5);
    //heroes[1].setHeroLocation(11,5);
    
    Displaytext();

    // set free warrior locs
    for(int i = 0; i < 14; i++)
    {
        if(warriors[i].isFree() == true) // if warrior is free
        {
            int r;
            int c;
            bool valid_loc = false; // create a bool of valid location for free warrior
            while(valid_loc == false) // while bool is still false
            {
                r = (rand()%24); // generate random location
                c = (rand()%15);
                
                bool overlap = false; // create a bool of overlap location
                for(int i = 0; i < 6; i++)
                {
                    //check if hero row, and col is the same as r, and c which is free warrior location
                    if(heroes[i].getRow() == r && heroes[i].getCol() == c)
                    {
                        overlap = true; // then overlap is true;
                    }
                }
                
                //if there is no overlap then we do the following
                if(overlap == false)
                {
                    if(warriors[i].hasShip() == false && board[r][c] != "w") // if warrior doesnt have ship and location is not water
                    {
                        valid_loc = true; // valid
                    }
                    else if(warriors[i].hasShip() == true && board[r][c] == "w") // if warrior has ship and location is water
                    {
                        valid_loc = true; // valid
                    }
                }
            }
            warriors[i].setLocation(r, c); // set the location
        }
    }
    
    //play the actual game
    gameEnd = false;
    string lastChoice;
    while(!gameEnd)
    {
        //phase 0: print map
        printMap(heroes[player].getRow(), heroes[player].getCol());
        //phase 1: moving or resting or consulting gods
        MainChoices();
        printMap(heroes[player].getRow(), heroes[player].getCol());
        cout << "" << endl;
        //phase 2: encounters
        ComputerMove();
        freeWarriorMoves();
        cout << "" << endl;
        encounters(player);
        cout << "" << endl;
        PlayerStatsReport();
        turnCount++;
        cout << "" << endl;
        cout << "Turn count is " << endl; 
        cout << turnCount << endl;
        
        if(heroes[player].getName() == "Stannis Baratheon")
        {
            endGame();
        }
        
        else
        {
            endOfStannis();
        }
        cout << "" << endl;
        //phase 6: random and predetermined events              
    }
    //gameEnd = true;
};

void Game::printMap(int row, int col) // print minimap
{
    int size = 7; // size of map
    int buffer = size / 2;
    string water = "~";
    string land = "*";
    string space = " ";
    int bufferNorth = 3; // set up buffer
    int bufferSouth = 3;
    int bufferEast = 3;
    int bufferWest = 3;
    
    if(row - bufferNorth < 0) // if row - buffer is out if bounds
    {
        bufferNorth = row; // buff will be row
    }
    
    if(col - bufferWest < 0) 
    {
        bufferWest = col;
    }
    
    if(row + bufferSouth > 25)
    {
        bufferSouth = 25 - row;
    }
    
    if(col + bufferEast > 16)
    {
        bufferEast =  16 - col;
    }
    
    for(int i = row - bufferNorth; i < row+bufferSouth+1; i++) // generate the minimap
    {
        for(int j = col - bufferWest; j < col+bufferEast+1; j++)
        {
            if(board[i][j] == "w") // replacing w with ~
            {
                cout << water << " ";
            }
            
            else if(board[i][j] == "p") // replacing p with *
            {
                 cout << land << " ";
            }
           else
           {
                cout << board[i][j] << " ";
           }
        }
            cout << endl;
    }
}

void Game::MainChoices() //display main choices, and call relevant functions
{
    int mainchoices; 
    int directionChoice;
    cout << "What you want to do?" << endl;
    cout << "1. Travel" << endl;
    cout << "2. Rest" << endl;
    cout << "3. Consult With The Gods" << endl;
    while(cin >> mainchoices)
    {
        if(mainchoices < 1 || mainchoices > 3)
        {
            cout << "Please pick a viable command" << endl;
        }
        
        if(mainchoices == 1)
        {
            moving(heroes[player].getRow(), heroes[player].getCol()); // move hero
            break;
        }
        
        if(mainchoices == 2)
        {
            restChoice(player); // rest function
            break;
        }
        
        if(mainchoices == 3)
        {
            consultGod(player); // consult function
            break;
        }
    }
}

bool onMap(int r, int c) // check if hero is in the map or not
{
    if((r < 25 && c < 16) && (r >= 0) && (c >= 0) ) // if hero is in bounds
    {
        return true; 
    }
    else // if hero is not in bound
    {
        return false;
    }
}

void Game::PlayerStatsReport()
{
    cout << "This is " << heroes[player].getName() << " status" << endl;
    cout << "Hero's army: " << endl;
    cout << heroes[player].getArmy() << endl;
    cout << "Hero's influence" << endl;
    cout << heroes[player].getInfluence() << endl;
    cout << "Hero's money " << endl;
    cout << heroes[player].getMoney() << endl;
}

void Game::moving(int currentRow, int currentColumn) // moving function
{
    int directionChoice;
    bool turnEnd = true;
    cout << "Where do you want to go from here?" << endl;
    cout << "1.North" << endl;
    cout << "2.South" << endl;
    cout << "3.East" << endl;
    cout << "4.West" << endl;
    cout << "Please pick a direction" << endl;
    
    while(cin >> directionChoice && turnEnd == true)
    {
        //North
        if(directionChoice == 1)
            {
                if(onMap(currentRow-1 , currentColumn) == true) // check if not out of bound
                {  
                    string map = board[currentRow-1][currentColumn]; // store the minimap into map variable
                    if(map == "w") // if map is water
                    {
                       map = "~";
                       if(map == "~")
                       {    
                           heroes[player].setMoney(heroes[player].getMoney()+10); // update money
                       }
                    }
                    else if(map == "p") // if map is land
                    {
                         map = "*";
                         if(map == "*")
                         {
                             
                            heroes[player].setMoney(heroes[player].getMoney()+10); //update money
                            heroes[player].setArmysize(heroes[player].getArmy()+10); // update army
                         }
                    }
                    cout << "current tile" << endl;
                    cout << map << endl;
                    if(map == "~" && (heroes[player].checkShip() == false)) //if map is water, and hero doesn;t have ship
                    {
                        heroes[player].setHeroLocation(currentRow + 1, currentColumn); // move back
                        cout << "You cant go into water" << endl;
                    }
                    heroes[player].setHeroLocation(currentRow - 1, currentColumn); // set location
                    break;
                }
                else // if location is out of bound
                {
                    cout << "Out of bounds, please pick another location" << endl;
                    cout << "2.South" << endl;
                    cout << "3.East" << endl;
                    cout << "4.West" << endl;
                    cout << "Please pick a direction" << endl;
                }
            }
            
            //south
        if(directionChoice == 2)
            {
                if(onMap(currentRow + 1, currentColumn) == true) // check if not out of bounds
                {   
                    string map = board[currentRow + 1][currentColumn]; // store the location to map
                    if(map == "w") // if map is w changes to ~
                    {
                       map = "~";
                       if(map == "~") // if map is water
                       {
                           heroes[player].setMoney(heroes[player].getMoney() +10); // increase money
                       }
                    }
                    else if(map == "p") // if map is p, changes to *
                    {
                         map = "*";
                         if(map == "*") // if map is land, increase money and army
                         {
                             heroes[player].setMoney(heroes[player].getMoney() + 20);
                             heroes[player].setArmysize(heroes[player].getArmy()+10);
                         }
                    }
                    cout << "current tile" << endl;
                    cout << map << endl;
                    if(map == "~" && (heroes[player].checkShip() == false)) // check whether hero have ship or not
                    {
                        cout << "You cant go into water" << endl; // if not
                         currentRow - 1; // hero cant move there
                    }
                    heroes[player].setHeroLocation(currentRow + 1, currentColumn); // set up the location
                    break;
                }
                else
                {
                    cout << "Out of bounds, please pick another location" << endl;
                    cout << "1.North" << endl;
                    cout << "3.East" << endl;
                    cout << "4.West" << endl;
                    cout << "Please pick a direction" << endl;
                }
            }
                //east
        if(directionChoice == 3) 
            {
                if(onMap(currentRow, currentColumn + 1)) // check if not out of bounds
                {
                    string map = board[currentRow][currentColumn + 1]; // store the location to map
                    if(map == "w") // if map is water, changes to ~
                    {
                       map = "~";
                       if(map == "~") // if map is water, add money
                       {
                           heroes[player].setMoney(heroes[player].getMoney() + 10);
                       }
                    }
                    else if(map == "p") // if map is land, changes to *
                    {
                         map = "*";
                         if(map == "*") // if map is land, add money
                         {
                             heroes[player].setMoney(heroes[player].getMoney() +20);
                             heroes[player].setArmysize(heroes[player].getArmy()+10);
                         }
                    }
                    cout << "current tile" << endl;
                    cout << map << endl;
                    if(map == "~" && (heroes[player].checkShip() == false)) // if location is water, and there is no ship
                    {
                        cout << "You cant go into water" << endl; // then cant go into water
                        currentColumn - 1;
                    }
                    heroes[player].setHeroLocation(currentRow, currentColumn + 1); // set location
                                        //turnEnd = true;
                    break;
                }
                else
                {
                    cout << "Out of bounds, please pick another location" << endl;
                    cout << "1.North" << endl;
                    cout << "2.South" << endl;
                    cout << "4.West" << endl;
                    cout << "Please pick a direction" << endl;                
                }
            }
                               //west 
        if(directionChoice == 4)
            {
                if(onMap(currentRow, currentColumn - 1)) // check if not out of bounds
                {
                    string map = board[currentRow][currentColumn - 1];
                    if(map == "w")
                    {
                       map = "~";
                       if(map == "~")
                       {
                           heroes[player].setMoney(heroes[player].getMoney()+10);
                       }
                    }
                    else if(map == "p")
                    {
                         map = "*";
                         if(map == "*")
                         {
                             heroes[player].setMoney(heroes[player].getMoney()+20);
                             heroes[player].setArmysize(heroes[player].getArmy()+10);
                         }
                    }
                    cout << "current location" << endl;
                    cout << map << endl;
                    if(map == "~" && (heroes[player].checkShip() == false))
                    {
                        cout << "You cant go into water" << endl;
                        currentColumn + 1;
                    }
                    heroes[player].setHeroLocation(currentRow, currentColumn - 1);
                                        //turnEnd = true;
                    break;
                }
                else
                {
                    cout << "Out of bounds, please pick another location" << endl;
                    cout << "1.North" << endl;
                    cout << "2.South" << endl;
                    cout << "3.East" << endl;
                    cout << "Please pick a direction" << endl;
                }
            }
            
        if(directionChoice < 1 || directionChoice > 4) // if player pick a out of range choice
            {
                cout << "Please choose a correct command" << endl;
            }
    } 
}

void Game::restChoice(int h)
{
    Hero cur_hero = heroes[h]; // store index of playing hero
    for(int i = 0 ; i< heroes[player].getWarriorsize(); i++) // loop through heroes' warrior size
    {
        Warrior cur_w = warriors[getWarrior(cur_hero.getWarriorByIndex(i))]; // store warriors of current hero to cur_w
        cur_w.setStrength(cur_w.getStrength()+1); // increase warrior of hero strength
    }
    cout << "Your allies' strength has increased by 1" << endl;
}

void Game::consultGod(int h)
{
    
    Hero cur_hero = heroes[h];
    for(int i = 0; i < heroes[player].getWarriorsize(); i++)
    {
        Warrior cur_w = warriors[getWarrior(cur_hero.getWarriorByIndex(i))]; //get warrior of current hero, store them to cur_w
        cur_w.setMorale(cur_w.getMorale() + 1); // increase morale
    }
    cout << "Your allies' morale has increased by 1" << endl;
}

//string Hero::getWarriorByIndex()
//int check for encounter
//user_encounter(int opponent)
//comp_encounter(int a, int b)
int Game::check_encounter(int h)
{
    //generate  a box that contains encounter zone
    int size_of_encounter = 5;
    int bufferNorth = 2; // buffer of the encounter zone
    int bufferSouth = 2; 
    int bufferEast = 2;
    int bufferWest = 2;
    
    int hr = heroes[h].getRow();
    int hc = heroes[h].getCol();
    
    if(heroes[player].getRow() - bufferNorth < 0) // if the current location is out of bound
    {
        bufferNorth = heroes[h].getRow(); // buffer becomes current hero row
    }
    
    if(heroes[player].getCol() - bufferWest < 0)
    {
        bufferWest = heroes[h].getCol();
    }
    
    if(heroes[player].getRow() + bufferSouth > 25)
    {
        bufferSouth = 25 - heroes[h].getRow();
    }
    
    if(heroes[player].getCol() + bufferEast > 16)
    {
        bufferEast =  16 - heroes[h].getCol();
    }
    
    //loop through heroes
    for(int i = 0; i < 6; i++)
    {
        if(i != h)
        {
            int r = heroes[i].getRow();
            int c = heroes[i].getCol();
            
            if(r >= hr - bufferNorth && r <= hr+ bufferSouth && c <= hc + bufferEast && c >= hc - bufferWest) // if there is hero inside the zone, return the index of that hero
            {
                return i;
            }
        }
    }
        
    //loop through free warrios
    for(int i = 0; i < 14; i++)
    {
        if(warriors[i].isFree() == true)
        {
            int r = heroes[i].getRow();
            int c = heroes[i].getCol();
        
            if(r >= bufferNorth && r<= bufferSouth && c <= bufferEast && c >= bufferWest) // if there is free warrior inside the zone
            {
                return i+7; // change index into free warrior
            }
        }
    }
    return -1;
}

void Game::choosewarriors(int h, int w)
{
    cout << "Choose 4 from the following 5:" << endl;
    int chooseOption;
    int wsize = heroes[h].getWarriorsize();
    string options[5];
    for(int i = 0; i< wsize; i++) // loop through size of warriors of hero
    {
       heroes[h].removeWarrior(heroes[h].getWarriorByIndex(i)); // remove warrior
        cout << i+1 << ". " << heroes[h].getWarriorByIndex(i) << endl; // generate the name of warriors of that hero
        options[i] = heroes[h].getWarriorByIndex(i); 
    }
    
    cout << heroes[h].getWarriorsize() + 1 << ". " << warriors[w].getName() << endl; // shows the fifth warrior
    options[4]  = warriors[w].getName(); // get that warrior name
    
    cout << "Enter number for which warrior you'd like to choose " << endl;
    int warrior_capacity = 4;
    int num_chosen = 0;
    int chosen[4];
    
    //ask user for their choices
    cout << "You have " << warrior_capacity - num_chosen << " free spots remaining" << endl;
        
    while(cin >> chooseOption && num_chosen < warrior_capacity)  
    {
        //loop through chosen and make sure that chooseOption isnt there
        //if so:
        bool ac = false;
        for(int i = 0; i < num_chosen; i++)
        {
            if(num_chosen == chosen[i])
            {
                ac = true;
            }
        }
        
        if (ac)
        {
            //error!!! choose again
            cout << "choose again " << endl;
        }
        else
        {
            // actually add the warrior
            cout << "You have " << warrior_capacity - num_chosen << " free spots remaining" << endl;
            chosen[num_chosen++] = chooseOption;
            heroes[h].addWarrior(options[chooseOption]);
            if (chooseOption == 5)
            {
                //set w's free status to false
                warriors[w].setFree(false);
            }
        }
    }
}

void Game::encounters(int h)
{
    int battleChoices;
    int opponent = -1;
    while((opponent = check_encounter(h)) > -1)
    {
        //warrior 
        if (opponent > 6) // if check encounter is more than 6, then its the free warrior encounter
        {
            //free warrior encountered
            opponent -= 7; // decrease the index to match the free warrior object
            if(heroes[player].getWarriorsize() == 4)
            {
                choosewarriors(h, opponent);
            }
            
            else
            {
                //
                warriors[opponent].setFree(false);
                heroes[h].addWarrior(warriors[opponent].getName());
            }
            // to end encounter, make warrior not free
            
        }
        //handle encounter
        cout << "Encounter happens" << endl;
        cout << "" << endl;
        cout << "A battle has started between " << heroes[h].getName() << " and " << heroes[opponent].getName() << endl; 
        cout << "" << endl;
        int heroTotalstrength; 
        cout << "encounter starts" << endl;
        cout << "you are given 3 choices" << endl;
        cout << "1. give a speech" << endl;
        cout << "2. buy them" << endl;
        cout << "3. battle them" << endl;
        while(cin >> battleChoices)
        {
            if(battleChoices > 3 || battleChoices < 0)
            {
                cout << "please pick a correct choice" << endl;
            }
                    
            if(battleChoices == 1) // if decided to give a speech 
            {
                int mainHeroInfluence = (heroes[player].getInfluence()) / 1600.0; //store in main hero influence
                int opposingHeroInfluence = 0.0; // variable to store the opposing hero influence.
                for(int i = 0; i < 14; i++) // loop through warriors index
                {
                    if(heroes[opponent].getWarriorByIndex(0) == warriors[i].getName()) // if opponent's first warrior name is equal to list of warriors
                    {
                        opposingHeroInfluence = (1 - (warriors[i].getMorale())/100.0); // then take the morale of that warrior object and do calc
                    }
                }
                
                int winningInfluenceProbrability = (mainHeroInfluence) * (opposingHeroInfluence); //multiplying them together
                
                if(winningInfluenceProbrability > 0 && winningInfluenceProbrability < 1) // if the probability is higher than 0 but less than 1
                {
                    int randomChoices = random()%100+1; // generate a number from 1 to 100
                    if((winningInfluenceProbrability)*100 >= randomChoices) // if the winning probability is higher or equal to the random number
                    {
                        cout << "you win the encounter" << endl; // win the encounter
                        //update stats
                        heroes[opponent].setHeroLocation(-1, -1); // opponent get send out of the map
                        heroes[opponent].setAlive(false);
                        heroes[player].setMoney(heroes[player].getMoney() + heroes[opponent].getMoney()); // acquire the stats of the deceased hero
                        heroes[player].setArmysize(heroes[player].getArmy() + heroes[opponent].getArmy());
                        heroes[player].setInfluence(heroes[player].getInfluence() + heroes[opponent].getInfluence());
                    }
                    
                    if((winningInfluenceProbrability)*100 <= randomChoices) // if the probability is lesser than the random generated number
                    {
                        heroes[player].setInfluence(heroes[player].getInfluence() / 2); // influence of hero gets divided by 2
                        //out of bounds
                        heroes[player].setHeroLocation(heroes[player].getRow() + 4, heroes[player].getCol() - 2); // move hero out of the encounter zone
                        OutOfEZ(player, opponent);
                        //tell the plaery they escaped to this location
                        cout << "you lost the encounter" << endl; // lost the encounter
                    }
                }
                if(winningInfluenceProbrability  <= 0.0) // if the probability is less or equal to zero 
                {
                    cout << "you lost the encounter" << endl; // lost automatically
                    heroes[player].setInfluence(heroes[player].getInfluence() / 2);
                    //out of bounds
                    heroes[player].setHeroLocation(heroes[player].getRow() + 4, heroes[player].getCol() - 2);
                    break;
                }
                            
                if(winningInfluenceProbrability >= 1.0) // if the probability is higher or equal to 1
                {
                    cout << "you win the encounter" << endl; // win automatically
                    //loss of og stats
                    heroes[opponent].setHeroLocation(-1, -1); // opponent get send out of the map
                    heroes[opponent].setAlive(false);
                    heroes[player].setMoney(heroes[player].getMoney() + heroes[opponent].getMoney()); // acquire the stats of the deceased hero
                    heroes[player].setArmysize(heroes[player].getArmy() + heroes[opponent].getArmy());
                    heroes[player].setInfluence(heroes[player].getInfluence() + heroes[opponent].getInfluence());
                    break;
                }
            }
                        
            if(battleChoices == 2) // if decided to buy
                {
                    int opposingHeroLoyalty;
                    int mainHeroMoney = (heroes[player].getMoney()) / 17800.0; // create var that calc the mainhero money
                    for(int i = 0; i < 14; i++) // loop through warrior
                    {
                        if(heroes[opponent].getWarriorByIndex(0) == warriors[i].getName()) // find corresponding opponent 1st warrior by comparing name
                        {
                            opposingHeroLoyalty = (1 - (warriors[i].getLoyalty())/100.0); // get the opponent 1st warrior loyalty, and do calc
                        }
                    }
                    int winningLoyaltyProbability = mainHeroMoney * opposingHeroLoyalty;
                    if(winningLoyaltyProbability > 0 && winningLoyaltyProbability < 1) // if the probability is larger than 0, and less than 1
                    {
                        int randomChoices = random()%100+1; // generate a number from 1 to 100
                        if((winningLoyaltyProbability)*100 >= randomChoices) // if the chance is larger than generated number
                        {
                            cout << "you win the encounter" << endl;
                            cout << heroes[opponent].getName() << " has lost the encounter" << endl;
                            heroes[opponent].setHeroLocation(-1,-1); // put opponent out of the map
                            heroes[opponent].setAlive(false);
                            heroes[player].setMoney(heroes[opponent].getMoney()); // get opponent money
                            heroes[player].setArmysize(heroes[opponent].getArmy()); // get opponent army
                            heroes[player].setInfluence(heroes[opponent].getInfluence()); // get opponent influence
                            break;
                        }
                    
                        if((winningLoyaltyProbability)*100 <= randomChoices) // if the chance is lesser than the generated number
                        {
                            cout << "you lost the encounter" << endl; // if lost
                            heroes[player].setMoney(heroes[player].getMoney() / 2); // money get divided by 2
                            heroes[player].setHeroLocation(heroes[player].getRow() + 4, heroes[player].getCol() - 2); //move hero away from the opponent
                            cout << "you ran away" << endl;
                            break;
                        }
                    }
                    
                    if(winningLoyaltyProbability <= 0.0)
                    {
                        cout << "you lost the encounter" << endl;
                        heroes[player].setMoney(heroes[player].getMoney() / 2);
                        heroes[player].setHeroLocation(heroes[player].getRow() + 4, heroes[player].getCol() - 2);
                        cout << "you ran away" << endl;
                        break;
                    }
                            
                    if(winningLoyaltyProbability >= 1.0)
                    {
                        cout << "you won the encounter" << endl;
                        cout << heroes[opponent].getName() << " has lost the encounter" << endl;
                        heroes[opponent].setHeroLocation(-1,-1);
                        heroes[opponent].setAlive(false);
                        heroes[player].setMoney(heroes[opponent].getMoney());
                        heroes[player].setArmysize(heroes[opponent].getArmy());
                        heroes[player].setInfluence(heroes[opponent].getInfluence());
                        break;
                    }
                    
                }
                        
            if(battleChoices == 3) 
            {
                int warriorChoices;
                int playerWarrior = -1; // index of the chosen warrior
                int opWarrior = -1;
                int p_score = heroes[player].getArmy();
                int o_score = heroes[opponent].getArmy();
                //cout << "yup" << endl;
                if(heroes[player].getWarriorsize() == 0)
                {
                    gameEnd = true;
                    endGame();
                    break;
                }
                else if(heroes[player].getWarriorsize()>1)
                {
                    //random choice
                    cout << "pick your warriors" << endl;
                    for(int i = 0; i < heroes[player].getWarriorsize(); i++)
                    {
                        cout << i+1 << ". " << heroes[player].getWarriorByIndex(i) << endl;
                    }
                    
                    while(cin>>warriorChoices)
                    {
                        //
                        break;
                    }
                    //
                    int strength = warriors[getWarrior(heroes[player].getWarriorByIndex(playerWarrior - 1))].getStrength();
                    p_score *= strength;
                    
                }
                else
                {
                    //only warrior
                   int strength = warriors[getWarrior(heroes[player].getWarriorByIndex(0))].getStrength();
                   p_score*= strength;
                }
                
                if (heroes[opponent].getWarriorsize() == 0)
                {
                    heroes[opponent].setHeroLocation(-1, -1);
                    heroes[opponent].setAlive(false);
                    cout << heroes[opponent].getName() << "have retired" << endl;
                    break;
                }
                else if(heroes[opponent].getWarriorsize()>1)
                {
                    opWarrior = random()%heroes[opponent].getWarriorsize();
                    int strength = warriors[getWarrior(heroes[opponent].getWarriorByIndex(opWarrior))].getStrength();
                    o_score *= strength;
                }
                else
                {
                    int strength = warriors[getWarrior(heroes[opponent].getWarriorByIndex(0))].getStrength();
                    o_score*= strength;
                }
                
                //now we know the battle actually happens
                if(p_score > o_score)
                {
                    cout << "Player wins" << endl;
                    cout << heroes[opponent].getName() << " has retired from the game" << endl;
                    heroes[opponent].setHeroLocation(-1, -1);
                    heroes[opponent].setAlive(false);
                }
                
                else if(o_score > p_score)
                {
                    cout << heroes[player].getName() << " has lost" << endl;
                    gameEnd = true;
                    endGame();
                }
                break;
            }
        }
    }
}

void Game::endOfStannis()
{
    string Respect;
    if (turnCount == 10) // if turn is 10
    {
        heroes[1].setHeroLocation(-1, -1); // set Stannis out of the map
        heroes[1].setAlive(false);
        cout << "Stannis has passed away" << endl;
        cout << "Press F to pay respect" << endl;
        while(cin >> Respect) // pay respect for him
        {
            if(Respect == "F" || Respect == "f")
            {
                cout << "Thank you for paying respect" << endl;
                cout << "The castle can be rebuilt, in time. It's not the walls that make a lord, it's the man" << endl;
                break;
            }
            
            else
            {
                cout << "please respect Stannis" << endl;
            }
        }
    }
}

void Game::endGame()
{
    
    if(heroes[player].getWarriorsize() == 0)
    {
        cout << "You have died" << endl;
        gameEnd = true;
    }
   
    if(heroes[player].getName() == "Stannis Baratheon")
    {
        if(turnCount == 10)
        {
            heroes[player].setHeroLocation(-1, -1);
            cout << "You have passed away" << endl;
            cout << "Thank you for playing" << endl;
            gameEnd = true;
        }
    }
}

void Game::OutOfEZ(int player, int opponent) // function that allows player hero to avoid opponent after encountering them once
{
    int Original_r = heroes[player].getRow(); // store the original locations
    int Original_c = heroes[player].getCol();
    
    int ez_buffer = 5; // buffer size
    
    int i = heroes[player].getRow() - ez_buffer - 1; // encounter zone of the hero
    int j = heroes[player].getCol() - ez_buffer - 1;
    
    if (i <= 0) // if the locations are out of bounds
    {
        i = 0;
    }
    if (j <= 0)
    {
        j = 0;
    }
    
    int r_bound = i + ez_buffer * 2 + 3; // finding new locations that is outside the encounter zone
    int c_bound = j + ez_buffer * 2 + 3;
    
    //bool valid = false;
    
    for(; i < r_bound; i++) // i is already declared 
    {
        for(; j < c_bound; j++) // j is already declared
        {
             if(onMap(i, j)) // check if location is valid
             {
                 heroes[player].setHeroLocation(i, j); // set hero to that new location
                 if(check_encounter(player) != opponent) // check if there is any encounter with opponent
                 {
                     return; // if yes, keep finding new position, if not return
                 }
             }
        }
    }
}


void Game::ComputerMove() 
{   
    for(int i = 0; i < 6; i++) // loop through hero
    {
        if (heroes[i].checkAlive() == true)
        {
            int randomChoices = random()%4+1; //randomize choice
            while(randomChoices)
            {
                if(heroes[i].getRow() == heroes[player].getRow())// if player has the same location as machine then break the loop
                {
                    break;
                }
                    
                if(heroes[i].getCol() == heroes[player].getCol()) // if player has the same location as machine, break the loop
                {
                    break;
                }
            //computer move
                if(randomChoices == 1) // if choice is 1 then move north
                    {
                        if(heroes[i].getRow()  - 1 == -1) // if computer move out of bound
                        {   
                            randomChoices = random()%4+1; // generate another choice
                        }
                        else
                        {
                            heroes[i].setHeroLocation(heroes[i].getRow()-1, heroes[i].getCol()); // if location is valid, set the location
                            break;
                        }
                }
            
                else if(randomChoices == 2) // if move south
                {
                        
                    if(heroes[i].getRow() + 1 == 25) // if computer move out of bound
                    {   
                        randomChoices = random()%4+1; // generate another choice
                    }
                    else
                    {
                        heroes[i].setHeroLocation(heroes[i].getRow() +1, heroes[i].getCol()); // if location is valid, set the location
                        break;
                    }
                }
            
                else if(randomChoices == 3) // if move east
                {
                    if(heroes[i].getCol() - 1 == -1) // if computer move out of bound
                    {
                        randomChoices = random()%4+1; // generate another choice
                    }
                    else
                    {
                        heroes[i].setHeroLocation(heroes[i].getRow(), heroes[i].getCol() -1); // if location is valid, set the location
                    break;
                    }
                }
            
                else if(randomChoices == 4) // if move west
                {
                    if(heroes[i].getCol() + 1 == 16) // if computer move out of bound
                    {
                        randomChoices = random()%4+1; // generate another choice
                    }
                    else
                    {
                        heroes[i].setHeroLocation(heroes[i].getRow(), heroes[i].getCol() + 1); // if location is valid, set the location
                        break;
                    }
                }
            }
        }
    }
}

void Game::freeWarriorMoves()
{
    for(int i = 0; i < 14; i++)
    {
        if(warriors[i].isFree() == true) // loop through warriors to find free warrior
        {
            int randomChoices = random()%4+1; //randomize choice
            while(randomChoices)
            {
                if(randomChoices == 1) // if choice is 1 then move north
                {
                    if(warriors[i].getRow()  - 1 == -1) // if warrior moves out of bound
                    {   
                        randomChoices = random()%4+1; // generate another choice
                    }
                    else
                    {
                        warriors[i].setLocation(heroes[i].getRow()-1, warriors[i].getColumn()); // if choice is valid then set the location
                        break;
                    }
                }
        
                else if(randomChoices == 2) //if move south
                {
                    
                    if(warriors[i].getRow() + 1 == 25) // if warrior moves out of bound
                    {   
                        randomChoices = random()%4+1; // generate another choice
                    }
                    else
                    {
                        warriors[i].setLocation(warriors[i].getRow() +1, warriors[i].getColumn()); // if choice is valid then set the location
                    break;
                    }
                }
        
                else if(randomChoices == 3)
                {
                    
                    if(warriors[i].getColumn() - 1 == -1) // if warrior moves out of bound
                    {
                        randomChoices = random()%4+1; // generate another choice

                    }
                    else
                    {
                        warriors[i].setLocation(warriors[i].getRow(), warriors[i].getColumn() -1); // if choice is valid then set the location
                    break;
                    }
                }
        
                else if(randomChoices == 4)
                {
                    if(warriors[i].getColumn() + 1 == 16) // if warrior moves out of bound
                    {
                        randomChoices = random()%4+1; // generate another choice

                    }
                    else
                    {
                        warriors[i].setLocation(warriors[i].getRow(), warriors[i].getColumn() + 1); // if choice is valid then set the location

                    break;
                    }
                }
            }
        }
    }
}

void Game::createBoard(string filename) // create map for the game
{
    ifstream fileOpen(filename); //read in map 
    string pre_row; // preprocessed row
    int countrow = 0;
    while(getline(fileOpen, pre_row))
    {
        string row = "";
        
        for(int i = 0; i < pre_row.length(); i+=2) // skips the coma
        {
            row += pre_row[i]; //copy content to row
        }
        for(int i = 0; i < row.length(); i++) // store row in board
        {
            board[countrow][i] = row[i];
        }
        countrow++;//increment row
    }
}    
    
void Game::setDG_location() //set random dragonglass location
{
    srand(time(NULL)); //seed randomness
    for(int i = 0; i < 5; i++) //loop through rows
    {
        int r = (rand()% 24); //seed random
        int c = (rand()%15);
        dG_location[i][0] = r; // store random numbers in location.
        dG_location[i][1] = c;
    }
}

void Game::readWarrior(string fileWarrior) // read in file warrior
{
    srand(time(NULL)); // randomness for free warrior
    ifstream fileOpen(fileWarrior);
    string warriorinfo;
    int counter = 0;
    while(getline(fileOpen, warriorinfo))
    {
        string arr[warriorinfo.length()];
        split(warriorinfo, ',', arr);
        bool wfree = false; // while read in line, if warrior is freedom is yes, bool is true. Same for other two
        if(arr[4] == "yes") 
        {
            wfree = true;
        }
        
        bool ship = false;
        if(arr[5] == "yes") // if warrior ship is yes
        {
            ship = true;
        }
        
        bool dg = false;
        if(arr[6] == "yes") // if warrior has dragon glass
        {
            dg = true;
        }
        
        Warrior W(arr[0], stoi(arr[1]), stoi(arr[2]), stoi(arr[3]), wfree, ship, dg); // put the info into the corresponding slot using the parameterized constructor
        warriors[counter++] = W;//keeps adding warriors info
    }
}

void Game::readHero(string herofile) //read in herofile
{

    ifstream fileOpen(herofile);
    string heroinfo;
    int herocounter = 0;
    while(getline(fileOpen, heroinfo))
    {
        string heroarr[heroinfo.length()]; // preprocessed string into array
        split(heroinfo, ',', heroarr); // split them
        
        string name = heroarr[0];
        int money = stoi(heroarr[1]);
        int influence = stoi(heroarr[2]);
        int army = stoi(heroarr[3]);
        int r = stoi(heroarr[8]) - 1;
        int c = stoi(heroarr[9]) - 1;
        bool ship = false;
        if(heroarr[10] == "yes") // if hero has ship
        {
            ship = true; // bool is true
        }
        string w[4];
        for(int i = 0; i < 4; i++)
        {
            if(heroarr[i+4] == "NULL")
            {
                w[i] = "";
            }
            else
            {
                w[i] = heroarr[i+4];
            }
        }
        Hero H(name, money, influence, army, w, r, c, ship); // store the info to the parameter
        heroes[herocounter++] = H; // keep adding hero
    } 
}


void Game::readCity(string cityfile) // read in city file
{
    ifstream fileOpen(cityfile); 
    string cityinfo;
    int citycounter = 0;
    while(getline(fileOpen, cityinfo))
    {
        string cityarr[cityinfo.length()];
        split(cityinfo, ',', cityarr); // split the cityinfo up, and store them into cityarr
        City C(cityarr[0], cityarr[1], stoi(cityarr[2]), stoi(cityarr[3]), stoi(cityarr[4])); // add info to the parameter
        cities[citycounter++] = C; // keeps adding info
    }
}

void Game::setpointhero() // ini starting points in the beginning
{
    for(int i = 0; i < 6; i++) // loop through hero
    {
        for(int j = 0; j < 25; j++) // loop through the cities
        {
            int points_to_assign = cities[j].getPoints();// store points into variable
            heroes[i].setHeropoints(points_to_assign); // assign the points to heroes
        }
    }
}

void Game::setStatusforCustomHero(string username)
{
    string name = username;
    string herowarriors[4] = {"","","",""}; // storing heroes for custom hero
    int money;
    int influence;
    int army_size;
    int starting_row;
    int starting_column;
    bool heroShip = false;
    
    cout << "Please input some stats" << endl;
    
    cout << "How much gold do you need?"<< endl;
    while(cin >> money)
    {
        if(money > 10000 || money < 0)
        {
            cout << "please dont break the game" << endl;
        }
        break;
    }
    
    cout << "How much influence do you need?" << endl;
    while(cin >> influence)
    {
        if(influence > 10000 || influence < 0)
        {
            cout << "Please dont break the game" << endl;
        }
        break;
    }
    
    cout << "What about your army size?" << endl;
    while(cin >> army_size)
    {
        if(army_size > 10000 || army_size < 0)
        {
            cout << "Please dont break the game" << endl;
        }
        break;
    }

    bool valid = false;
    while(!valid) 
    {
        //prompt 
        cout << "Please pick your location(row first, and then column)" << endl; 
        
        //store
        cout << "Row:" << endl;
        cin >> starting_row;
        cout << "Col:" << endl;
        cin >> starting_column;
        
        //check
        bool rcBounds = false;
        bool noOverlap = true;
        if(!((starting_row > 25 || starting_row < 0) || (starting_column > 16 || starting_column < 0)))
        {
            rcBounds = true; // custom hero's location is not out of bound
            for(int i = 0; i < 6; i++) // check if hero location is the same as default hero
            {
                if(heroes[i].getRow() == starting_row && heroes[i].getCol() == starting_column)
                {
                    cout << "This location is already occupied" << endl;
                    noOverlap = false;
                }
            }
        }
        
        if(!(rcBounds && noOverlap)) // if there is no overlap but picked out of bound location
        {
            cout << "Please pick another location" << endl; 
        }
        else // location is valid
        {
            valid = true;
        }
    }
   Hero h7(name, money, influence, army_size, herowarriors, starting_row, starting_column, heroShip);
   heroes[6] = h7; // overwrite the last hero
}

 
//GETTERS
int Game::getHero(string heroname) // get hero object based on hero name
{
    for(int i = 0; i < 7; i++) //loop through hero
    {
        if(heroes[i].getName() == heroname) // find corresponding name
        {
            return i; // return object
        }
    }
}

int Game::getWarrior(string Warriorname) // get warrior object based on warrior name
{
    for(int i = 0; i < 14; i++) // loop through warriors
    {
        if(warriors[i].getName() == Warriorname) // get warrior name
        {
            return i; // return the warrior object
        }
    }
}

int Game::getCity(string Cityname) //get city object based on city name
{
    for(int i = 0; i < 26; i++) //loop through city
    {
        if(cities[i].getName() == Cityname); // find corresponding name
        {
            return i; // return city object
        }
    }
}

bool Game::isDG(int row, int column) // check if there is dragon glass or not
{
    for(int i = 0; i < 5; i++) //loop through row
    {
        for(int j = 0; j < 2; j++) // through columns
        {
            if (i == row && j == column)
            {
                return true;
            }
        }
    }
    return false;
}

