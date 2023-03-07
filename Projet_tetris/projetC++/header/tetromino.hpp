#pragma once 

#include "board.hpp"

using namespace std;

class tetromino{
    int x;
    int y;
    int rotateState;
    arraysix offset;
    sf::Color color;

    public: 
        int value;
        //constructor
        tetromino(){};
        tetromino(int value,arraysix offset,sf::Color color):
            value{value},rotateState{0},offset(offset),color(color){};
            
        //method associated to the tetromino
        void setCoord(int x,int y);
        arrayeight getCoords();
        void draw(sf::RenderWindow& window);
        void updateRight(Board& board);
        void updateLeft(Board& board);
        void lock(Board& board);
        void updateDown(Board& board,bool& newpiece,bool ghost=false);
        void harddrop(Board& board,bool& newpiece);
        void drawGhost(Board& board,sf::RenderWindow& window);
        void rotate(bool clockwise=true);
        bool updateRotate(Board& board,bool clockwise=true);
        void updateKickRotate(Board& board, bool clockwise=true);
        bool inHiddenLayer();
};

extern tetromino I;
extern tetromino O;
extern tetromino S;
extern tetromino Z;
extern tetromino J;
extern tetromino L;
extern tetromino T;

//Color we use in different 

extern sf::Color colorarray[8]; 
extern array<tetromino,7> tetromino_array;


//WallKick value for the tetrominos
extern wallkick zero;
extern wallkick one;
extern wallkick two;
extern wallkick three;

extern wallkick zeroone_I;
extern wallkick onetwo_I;
extern wallkick twothree_I;
extern wallkick threezero_I;

extern array<wallkick,4> wallkick_array;
extern array<wallkick,4> wallkick_array_I;

class bag_tetromino{
    public:
        array<int,3> bag;
        int start;
        default_random_engine generator;
        uniform_int_distribution<int> distribution;

        //By putting the random generator in the attribut, we avoid creating a new random generator at each frame

        bag_tetromino(){
            std::random_device r;
            default_random_engine generator(r());
            uniform_int_distribution<int> distribution(0,1000);
            for(auto& i:bag){
                i=(distribution(generator)%6);
            }
            this->start=0;
            this->generator=generator;
            this->distribution=distribution;
        }
        int get_value();
        void drawBag(sf::RenderWindow& window);
};