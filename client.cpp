// Matan Shlomo Ohayon, ID : 207696907
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <thread>
#include "std_lib_facilities.h"
#include <mutex>

// Mutex is used to ensure that the message interface orints correctly.
mutex client_mutex;

// A function yhay handels the entire client side.
void client (int client_socket){
    
    // Using threads to allow messages to flow from the server.
    thread([client_socket](){
        char buffer[1024];
        
        while(true){
            
            memset(buffer, 0, sizeof(buffer));
            
            // Receving a message from ther server.
            int bytes_received = recv(client_socket, buffer, sizeof(buffer),0);
            if( bytes_received > 0){
                // Starting a mutex lock.
                unique_lock<mutex> lock(client_mutex);
                char* sender_client_name = strtok(buffer, " ");
                char* rest_of_the_message = strtok(nullptr, "");

                // Clear the row for nice interface.
                cout << "\r" << string(50, ' ') << "\r";
        
                // Printing the received message.
                cout << "Incoming message from " << sender_client_name << " : " << rest_of_the_message << endl;

                // Checks whether the message is an error message that forces am immediate exit.
                if(strcmp(rest_of_the_message,"The name is already in use.") == 0){
                    close(client_socket);
                    _Exit(1);
                } 

                if(strcmp(rest_of_the_message,"Sorry, the server was shut down.") == 0){
                    close(client_socket);
                    _Exit(1);
                }


            

                cout << "Enter your message : " << flush;
            }    
        }
    }).detach();
    
    while(true){

        string message;
        cout << "Enter your message : ";
        getline(cin,message);

        // If the client tpes exit the program ends.
        if(message == "exit"){
            break;
        }

        send(client_socket, message.c_str(), message.size(), 0);
        message = "";
    }
}

int main(int argc, char* argv[]) {  
    
    if (argc < 3) {
        cerr << "Usage: ./client <server> <client_name>" << endl;
        return 1;
    }

    string server_ip = argv[1];
    string client_name = argv[2];

    // Setting the client socket.
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if( client_socket == -1){
        cerr << "Failed to ceat socket." << endl; 
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;    
    server_addr.sin_port = htons(4444);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    // Connecting to the server.
    if(connect(client_socket,(sockaddr *)&server_addr, sizeof(*&server_addr)) < 0 ){
        perror("Failed to connect to server.");
        return -1;
    }
    // Send to the server the name of the client.
    send(client_socket, client_name.c_str(),client_name.size(),0);

    cout << "Connected to server : " << server_ip << " as : " << client_name << endl;

    // Calling to the client function.
    client(client_socket);

    //  Closeing the socket.
    close(client_socket);

    return 1;

}

//complie:  g++ client.cpp -o client
//run: 
//client: ./client 127.0.0.1 <client_name>


