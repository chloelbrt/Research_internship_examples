#include "header/tetromino.hpp"
#include "header/drawing.hpp"
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

using namespace std;


int main(int argc, char* argv[])
{
    sf::TcpListener listener;
    sf::TcpSocket client;
    if (argc==1){
        cerr << "Client or server missing" << endl;
        return 0;
    }
    string client_server=argv[1];
    if (client_server=="server"){
        cout << "Waiting for connection" << endl;
        // bind the listener to a port
        if (listener.listen(53000) != sf::Socket::Done){
            cerr << "The port you want to use is already used" << endl;
            return 0;
        }
        listener.accept(client);
    }
    else if(client_server=="client"){
        if(argc!=3){
            cerr << "The server ip address is missing" << endl;
            return 0;
        }
        sf::IpAddress serverAdress=argv[2];
        client.connect(serverAdress,53000);
    }
    client.setBlocking(false);

    cout << "Connection established" << endl;
    Board board;
    sf::RenderWindow window(sf::VideoMode(CELLSIZE*(COLUMN+9), CELLSIZE*ROW), "TETRIS GAME");

    window.setFramerateLimit(200);
    int speed=1000;

    //Pieces used in the game 
    
    tetromino piece;
    tetromino ghost;
    tetromino hold;
    tetromino buffer; //Sole purpose is to swap hold and current piece
    
    //Send and receive buffer

    int receive;
    size_t sent_size;
    size_t size;

    //boolean to check if I can lock the piece and if I have to create a new piece
    bool tolock=false;
    bool newpiece=true;
    bool keyhold=false;
    bool holding=false;

    int levelCounter{0};
    int lineCleared{0};
    int score{0};
    int lineSend;
    int choice;
    int x_value;

    bag_tetromino bag;
    sf::Clock gameTime;
    sf::Clock clockSpeed;

    while (window.isOpen())
    {
        while(true){
            if(board.inHiddenLayer()) break;
            if (newpiece){
                choice=bag.get_value();
                piece=tetromino_array[I_tetromino];
                x_value=(choice==I_tetromino)?int(COLUMN/2)+1:int(COLUMN/2);
                piece.setCoord(x_value,2);
                newpiece=false;
            }
            sf::Event e;
            while (window.pollEvent(e))
            {
                if (e.type == sf::Event::Closed)
                    window.close();

                //For debugging purpose, decomment this to place blocks in the grid with the click of the mouse 
                
                // else if(e.type==sf::Event::MouseButtonPressed){
                //     if(e.mouseButton.button==sf::Mouse::Left){
                //         int x=sf::Mouse::getPosition(window).x/CELLSIZE; 
                //         int y=sf::Mouse::getPosition(window).y/CELLSIZE;
                //         board.setCell(x,y+3,6);
                //     }
                // }

                else if (e.type== sf::Event::KeyPressed){
                    if (e.key.code==sf::Keyboard::Right||e.key.code==sf::Keyboard::D){
                        piece.updateRight(board);
                    }
                    else if (e.key.code==sf::Keyboard::Left||e.key.code==sf::Keyboard::Q){
                        piece.updateLeft(board);
                    }
                    else if (e.key.code==sf::Keyboard::Down||e.key.code==sf::Keyboard::S){
                        piece.updateDown(board,newpiece); 
                    }
                    else if(e.key.code==sf::Keyboard::Up||e.key.code==sf::Keyboard::X){
                        piece.updateKickRotate(board);
                    }
                    else if(e.key.code==sf::Keyboard::Space){
                        piece.harddrop(board,newpiece);
                    }
                    else if(e.key.code==sf::Keyboard::C){
                        int offset_x=(piece.value==I_tetromino)?5:4;
                        int value=piece.value;  
                        if(piece.inHiddenLayer()){
                            if(holding){
                                piece=hold;
                                x_value=(piece.value==I_tetromino)?int(COLUMN/2)+1:int(COLUMN/2);
                                piece.setCoord(x_value,2);
                            }else{
                                holding=true;
                                newpiece=true;
                            }
                            hold=tetromino_array.at(value);
                            hold.setCoord(COLUMN+offset_x,9);
                        }
              
                    }
                    else if(e.key.code==sf::Keyboard::Z){
                        piece.updateKickRotate(board,false);
                    }
                    else if(e.key.code==sf::Keyboard::Escape){
                        window.close();
                    }
                
                
                }
            }         

            if(clockSpeed.getElapsedTime().asMilliseconds()>speed){
                clockSpeed.restart();
                piece.updateDown(board,newpiece);
            }

            if(levelCounter>=10){
                speed/=1.5;
                levelCounter=levelCounter%10    ;
                cout << "Increase speed !" << endl;
            }
            
           
            score+=board.lineClear(levelCounter,lineCleared,lineSend);
            if (lineSend!=0) cout << lineSend << endl;
            if(lineSend>1){
                lineSend-=1;
                client.send(&lineSend,2,sent_size);
            }
            sf::Socket::Status status = client.receive(&receive,2,size);
            if(status==sf::Socket::Done){
                cout << "received" << endl;
                board.addLine(receive);
                receive=0;
            }
            drawScoreBoard(window,lineCleared,score);
            drawGrid(window);
            hold.draw(window);
            piece.draw(window);
            piece.drawGhost(board,window);
            bag.drawBag(window);
            drawLocked(board,window);
            window.display();
            window.clear();
        }
        sf::Event e;
        while(window.pollEvent(e)){
            if (e.type==sf::Event::Closed){
                window.close();
            }
            if (e.type==sf::Event::KeyPressed){
                if (e.key.code==sf::Keyboard::Escape){
                    window.close();
                }
            }
        }
        drawGrid(window);
        drawEnd(board,window);
        window.display();
        window.clear();

    }
    return 0;
}