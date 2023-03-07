#include <iostream>
#include <SFML/Network.hpp>

using namespace std;


int main(int argc,char* argv[]){
    try{
        while (true){
            size_t bytes_received;
            char data[10]="bonjour";
            char receive[10];
            sf::UdpSocket socket;
            if(socket.bind(54000) != sf::Socket::Done){
                throw "Binding error";
            }   
            sf::IpAddress recipient=argv[1];
            unsigned short port;
            socket.send(data,10,recipient,54000);
            socket.receive(receive,10,bytes_received,recipient,port);
        }
    
    }
    catch(string e){
        cout << e << endl;
    }
   
}
  
