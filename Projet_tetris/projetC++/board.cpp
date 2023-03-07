#include "header/board.hpp"
#include <time.h>
using namespace std;

void Board::setCell(int x,int y, int colorindex){
    cell.at(y).at(x)=1;
    color.at(y).at(x)=colorindex;
};
int Board::getColor(int x,int y){
    return color.at(y).at(x);
};
int Board::getValue(int x,int y){
    return cell.at(y).at(x);
};

bool Board::inBound(int x,int y){
    if(x<=-1||x>=COLUMN) return false;
    else if (y>=ROW+3) return false;
    return true;
};

bool Board::inHiddenLayer(){
    for (int i = 0; i < COLUMN; i++)
        for (int j=0;j<3;j++)
                if(this->getValue(i,j)) return true;
    return false;
    
}

//Return the score added by clearing the line
int Board::lineClear(int& levelCounter,int& lineCleared, int& lineSend){

    int sum{0};
    int multipleLine{0};
    array<int,COLUMN> fullLine;
    array<array<int,COLUMN>,ROW+3> update_cell;
    array<array<int,COLUMN>,ROW+3> update_color;
    update_cell.fill({0,0,0,0,0,0,0,0,0,0});
    update_color.fill({-1,-1,-1,-1,-1,-1,-1,-1,-1,-1});
    fullLine.fill(1);
    for (int y=ROW+2;y>=3;y--){
        if(cell.at(y)==fullLine){
            multipleLine++;
            levelCounter++;
            lineCleared++;
        }
    }
    if(multipleLine!=0){
        int line{ROW+2};
        for (int z=ROW+2;z>=3;z--){
            if(cell.at(z)!=fullLine){
                update_cell.at(line)=cell.at(z);
                update_color.at(line)=color.at(z);
                line--;
            }
        }
        this->cell=update_cell;
        this->color=update_color;
    }
    lineSend=multipleLine;
    if (multipleLine==1) return 40;
    else if(multipleLine==2) return 100;
    else if(multipleLine==3) return 300;
    else if(multipleLine==4) return 1000;
    return 0;
};

void Board::addLine(int lineSend){
    srand(time(NULL)*9);
    int index=rand()%10;
    array<array<int,COLUMN>,ROW+3> update_cell;
    array<array<int,COLUMN>,ROW+3> update_color;
    update_cell.fill({0,0,0,0,0,0,0,0,0,0});
    update_color.fill({-1,-1,-1,-1,-1,-1,-1,-1,-1,-1});
    array<int,COLUMN> line;
    array<int,COLUMN> linecolor;
    for (int i=0;i<COLUMN;i++){
        if(i!=index){
            line[i]=1;
            linecolor[i]=7;
        }
    }
    for (int z=3;z<=ROW+2;z++){
        update_cell.at(z-lineSend)=cell.at(z);
        update_color.at(z-lineSend)=color.at(z);
    }
    for (int y=ROW+2;y>ROW+2-lineSend;y--){
        update_cell.at(y)=line;
        update_color.at(y)=linecolor;
    } 
    this->cell=update_cell;
    this->color=update_color;

};

