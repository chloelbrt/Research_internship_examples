#include "header/tetromino.hpp"
#include "header/drawing.hpp"

using namespace std;
//Board element

//Color used for the different tetrominos
sf::Color colorarray[8] = {
    sf::Color(234, 20, 28 ,200), //red
    sf::Color(255, 200, 46 ,200), //orange
    sf::Color(254, 251, 52 ,200), //yellow
    sf::Color(83, 218, 63 ,200), //green
    sf::Color(1, 237, 250 ,200), //cyan
    sf::Color(65,105,225,200), //blue
    sf::Color(221, 10, 178 ,200), //purple
    sf::Color(125,125,125), // grey
};


//Different tetromino pieces
tetromino Z(Z_tetromino,{0,-1,-1,-1,1,0},colorarray[Z_tetromino]);
tetromino L(L_tetromino,{-1,0,1,0,1,-1,},colorarray[L_tetromino]);
tetromino O(O_tetromino,{-1,0,-1,-1,0,-1},colorarray[O_tetromino]);
tetromino S(S_tetromino,{-1,0,0,-1,1,-1},colorarray[S_tetromino]);
tetromino I(I_tetromino,{-3,0,-2,0,-1,0},colorarray[I_tetromino]);
tetromino J(J_tetromino,{-1,0,-1,-1,1,0},colorarray[J_tetromino]);
tetromino T(T_tetromino,{-1,0,0,-1,1,0},colorarray[T_tetromino]);

//Defintion of the array where we get our tetrominos
array<tetromino,7> tetromino_array({Z,L,O,S,I,J,T});

//Definition of the wallkick array 

wallkick zero({-1,0,-1,-1,0,2,-1,2});
wallkick one({1,0,1,1,0,-2,1,-2});
wallkick two({1,0,1,-1,0,2,1,2});
wallkick three({-1,0,-1,1,0,-2,-1,-2});

wallkick zero_I({-2,0,1,0,-2,1,1,-2});
wallkick one_I({-1,0,2,0,-1,-2,2,1});
wallkick two_I({2,0,-1,0,2,-1,-1,2});
wallkick three_I({1,0,-2,0,1,2,-2,-1});

array<wallkick,4> wallkick_array({zero,one,two,three});
array<wallkick,4> wallkick_array_I({zero_I,one_I,two_I,three_I});

void tetromino::setCoord(int x,int y){
    this->x=x;
    this->y=y;
};

arrayeight tetromino::getCoords(){
    arrayeight ret;
    for (int j=0;j<3;j++){
        ret.at(j).at(0)=x+offset.at(j).at(0);
        ret.at(j).at(1)=y+offset.at(j).at(1);
    }
    ret.at(3).at(0)=x;
    ret.at(3).at(1)=y;
    return ret;
};

void tetromino::draw(sf::RenderWindow& window){
    for(auto coords:this->getCoords()){
        drawPixel(coords[0],coords[1],color,window);
    }

};

void tetromino::updateRight(Board& board){
    bool toupdate=true;
    int buffer{0};
    for (auto coords: this->getCoords()){
        buffer=coords[0]+1;
        if(buffer==COLUMN||board.getValue(buffer,coords[1])){
            toupdate=false;
            break;
        }
    }
    if (toupdate) x++;
};

void tetromino::updateLeft(Board& board){
    bool toupdate=true;
    int buffer{0};
    for (auto coords: this->getCoords()){
        buffer=coords[0]-1;
        if(buffer==-1||board.getValue(buffer,coords[1])){
            toupdate=false;
            break;
        }
    }
    if (toupdate) x--;
};

void tetromino::lock(Board& board){
    for(auto coords:this->getCoords()){
        board.setCell(coords[0],coords[1],value);
    }
};

void tetromino::updateDown(Board& board,bool& newpiece,bool ghost){
    bool toupdate=true;
    int buffer=0;
    for (auto coords: this->getCoords()){
        buffer=coords[1]+1;
        if(buffer==ROW+3||board.getValue(coords[0],buffer)){
            if(!ghost) this->lock(board);
            toupdate=false;
            newpiece=true;
            break;
        }
    }
    if(toupdate) y++;
}

void tetromino::harddrop(Board& board,bool& newpiece){
    while(!newpiece) this->updateDown(board,newpiece);
};

void tetromino::drawGhost(Board& board,sf::RenderWindow& window){
    bool boolbuffer=false;
    tetromino ghost(*this);
    while(!boolbuffer) ghost.updateDown(board,boolbuffer,true);
    ghost.color.a = 100;
    ghost.draw(window);
};

void tetromino::rotate(bool clockwise){
    int buffer{0};
    int sign=2*clockwise-1;
    int two{sign*2};
    if (value!=O_tetromino){
        for(int i=0;i<3;i++){
            buffer=this->offset.at(i).at(0);
            this->offset.at(i).at(0)=-sign*this->offset.at(i).at(1);
            this->offset.at(i).at(1)=sign*buffer;
        }
        if(value==I_tetromino){
            if(rotateState==clockwise*0+!clockwise*1) this->setCoord(x-sign,y+two);
            if(rotateState==clockwise*1+!clockwise*2) this->setCoord(x-two,y-sign);
            if(rotateState==clockwise*2+!clockwise*3) this->setCoord(x+sign,y-two);
            if(rotateState==clockwise*3+!clockwise*0) this->setCoord(x+two,y+sign);
        }
    }
    if (clockwise) rotateState=(rotateState+1)%4;
    else rotateState=(rotateState==0)?3:rotateState-1;

};

bool tetromino::updateRotate(Board& board,bool clockwise){
    bool toupdate=true;
    tetromino tetrobuffer(*this);
    tetrobuffer.rotate(clockwise);
    for (auto coords: tetrobuffer.getCoords()){
        if(!board.inBound(coords[0],coords[1])||board.getValue(coords[0],coords[1])){
            toupdate=false;
            break;
        }
    }
    if(toupdate) this->rotate(clockwise);
    return toupdate;
};

void tetromino::updateKickRotate(Board& board, bool clockwise){
    auto wallkicks=wallkick_array;
    int offset_x{0},offset_y{0},index{rotateState};
    tetromino tetrobuffer(*this);
    if (value==I_tetromino) wallkicks=wallkick_array_I;
    if (!clockwise){
        for(int i=0;i<4;i++) wallkicks.at(i).flipsigned();
        index=(rotateState==0)? 3:rotateState-1;
    }
    if (!this->updateRotate(board,clockwise)){
        for(auto offset:wallkicks[index].container){ 
            tetrobuffer.setCoord(x+offset[0],y+offset[1]);
            if (tetrobuffer.updateRotate(board,clockwise)){
                offset_x=offset[0];
                offset_y=offset[1];
                break;
            }
        }
        this->setCoord(x+offset_x,y+offset_y);
        this->updateRotate(board,clockwise);
   }
};

bool tetromino::inHiddenLayer(){
    arrayeight coords=this->getCoords();
    for(auto coord:coords){
        if (coord.at(1)<3) return true;
    }
    return false;
}


int bag_tetromino::get_value(){
    int ret=bag.at(start);
    bag.at(start)=(distribution(generator)%6);
    start++;
    start=start%3;
    return ret;
}

void bag_tetromino::drawBag(sf::RenderWindow& window){
    tetromino pieceBuffer;
    int index,tetromino_value;
    float offset_x,offset_y;
    for(int i=0;i<3;i++){
        index=start+i;
        tetromino_value=bag.at(index%3);
        pieceBuffer=tetromino_array.at(tetromino_value);
        offset_x=(tetromino_value==I_tetromino)?5:4;
        offset_y=(tetromino_value==I_tetromino)? offset_y+2: offset_y+3;
        pieceBuffer.setCoord(COLUMN+offset_x,12+offset_y);
        pieceBuffer.draw(window);

    }
}