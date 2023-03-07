#pragma once 

#include "global.hpp"

using namespace std;

class Board{

    array<array<int,COLUMN>,ROW+3> cell;
    array<array<int,COLUMN>,ROW+3> color;
    public:

        Board(){
            cell.fill({0,0,0,0,0,0,0,0,0,0});
            color.fill({-1,-1,-1,-1,-1,-1,-1,-1,-1,-1});
        }
        void setCell(int x,int y,int colorindex);
        int getColor(int x,int y);
        int getValue(int x,int y);
        bool inBound(int x,int y);
        bool inHiddenLayer();
        int lineClear(int& lineCounter,int& lineCleared,int& lineSend);
        void addLine(int lineSend);
};
