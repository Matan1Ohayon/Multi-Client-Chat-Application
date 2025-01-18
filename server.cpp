// Matan Shlomo Ohayon, ID : 207696907
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <pthread.h>
#include <thread>
#include "std_lib_facilities.h"

// Using map to store client names and their socket numbers.
map<string,int> clients;

// A function that handles the entire server side.
void handle_client(int client_socket){

    char buffer[1024];
    string client_name;
    
    // Receiving the client name.
    memset(buffer,0,sizeof(buffer));
    if(recv(client_socket, buffer, sizeof(buffer),0) > 0){
        client_name = buffer;
        // Makes sure to use capital letter.
        if(!client_name.empty()) client_name[0] = toupper(client_name[0]);
    } else {
        close(client_socket);
        return;
    }
    // If the client names is already in used sending error to the client.
    if(clients.find(client_name) != clients.end()){
        string s_err = "SERVER_ERROR The name is already in use.";
        send(client_socket, s_err.c_str(), s_err.size(),0);
        close(client_socket);
        return;
    }
    // Print the connected client to the server screen.
    cout << "Client connected : " << client_name << endl;
    clients[client_name] = client_socket;

    // Open for get messages from clients
    while(true){

        memset(buffer,0,sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer),0);

        // Disconnects clients who quested to disconnect.
        if(bytes_received <= 0 || string(buffer).find("exit") == 0){
            cout << "Client disconnected : " << client_name << endl;
            close(client_socket);
            clients.erase(client_name);
            break;
        }

        string message = string(buffer).substr(0, bytes_received);

        // Separates the messages into parts.
        istringstream iss(message);
        vector<string> words;
        string word;
        while(iss >> word){
            words.push_back(word);
        }

        // Checks that the first word is the "send" command.
        if(words[0] == "send" || words[0] == "Send" || words[0] == "SEND" ){
            // Calculates the name of the client to whom the message should be sent.
            string other_client_name= words[1];
            if(!other_client_name.empty()) other_client_name[0] = toupper(other_client_name[0]);
            if(clients.find(other_client_name) != clients.end()){
                
                // The rest of the message.
                string relmes = client_name + " ";
                for(int i=2; i<words.size(); i++){
                    relmes.append(" "+words[i]);
                }
                int other_client_socket = clients[other_client_name];
                cout << client_name << " sent a message to "  << other_client_name << endl;
                // Sends the message to the other client.
                send(other_client_socket, relmes.c_str(),relmes.size(),0);


            }else{
                // if the other client not found in the clients list.
                string s_err1 = "SERVER_ERROR " + other_client_name + " not found.";
                send(client_socket, s_err1.c_str(), s_err1.size(),0);
                continue;
            }

        }else if (words[0] == "list" || words[0] == "List" || words[0] == "List" )
        {
            string list_of_clients = "SERVER \nConnected clients : \n";
            for(auto p : clients){
                if(p.first != client_name){
                    list_of_clients.append("# " + p.first + "\n");
                }
            }
            send(client_socket, list_of_clients.c_str(), list_of_clients.size(),0);

        }else{
            // If the client wrote a different word than the "send" command.
            string s_err2 = "SERVER_ERROR Undefined function";
            send(client_socket, s_err2.c_str(), s_err2.size(),0);
            continue;
        }
    }
}

void listen_for_exit(int server_socket) {
    string command;
    while (true) {
        cin >> command;
        if (command == "exit") {
            cout << "Server is shutting down..." << endl;
            if(clients.size() > 0){
                cout << "Disconnected the clients..." << endl;
                for(auto p : clients){
                    string s_err = "SERVER_ERROR Sorry, the server was shut down.";
                    send(p.second, s_err.c_str(), s_err.size(),0);
                    close(p.second);
                }
            }
            close(server_socket);
            _Exit(1);
            break;
        }
    }
}
    
int main() {  
    
    // Setting the server socket.
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        cerr << "Failed to create socket." << endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr,0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;    
    server_addr.sin_port = htons(4444);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //Bind the socket to the address
    if(bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        cerr << "Bind failed" << endl;
        return -1;
    }
    
    //Listen on the sockets
    if(listen(server_socket,10) == -1){
        cerr << "Listen failed" << endl;
        return -1;
    }

    cout << "Server listening on 127.0.0.1 : 4444 ." << endl; 
    cout << "======================================" << endl; 
    
    
    // Start a thread to listen for the exit command
    thread exit_thread(listen_for_exit, ref(server_socket));

    while (true)
    {

        sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_size);
        
        if(client_socket == -1){
            cout << "Failed to accept connection." << endl;
            continue;
        }
        // Using thread for each client.
        thread(handle_client, client_socket).detach();

    }

    exit_thread.join();

    close(server_socket);
    return 0;
    
}

//compile:  g++ server.cpp -o server
//run: 
//server: ./server
