#include "Server.h"


Server::Server(int port_num) {
    open_port_fd=0;
    client_port_fd=0;
    memset(&client_addr, 0, sizeof(client_addr));
    memset(&server_addr, 0, sizeof (server_addr));//initialize all structs to 0

    buffer= (char*)malloc(sizeof(char)*SOCKET_BUF_SIZE);

    if((open_port_fd = socket(AF_INET, SOCK_STREAM, 0))<0)// initializes socket of IP type and of reliable connection
        cout<<"ERROR CREATING SOCKET\n";
    bool yes=true;
    setsockopt(open_port_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)); //sets option on socket api that allows kernel to reuse sockets that are still in memory

    server_addr.sin_family = AF_INET; //AF_INET is the family of ip address, for our case an IPV4 address
    //sets server_addr to
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);// htonl is network long, format is in network byte order
    server_addr.sin_port = htons(port_num);//htons is host to network short, the format of the short is in network byte order

    bind(open_port_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));//bind binds the address info to the socket
    //listen sets port to listen for client connections
    // 1 allows only one client machine to be connected on the port
    if (listen(open_port_fd, 1) == -1) {
        perror("listen");
        exit(1);
    }
    socklen_t client_addr_size=sizeof(client_addr);
    client_port_fd = accept(open_port_fd, (struct sockaddr*)&client_addr,&client_addr_size); //completes connection with client

    if (client_port_fd==-1)
        cout<<"ERROR IN ACCEPT\n";
    //string msg="---------Hello you have connected to the 5 in a row server---------\n";
    //send(client_port_fd, msg.c_str(), msg.size(), 0);//0 means no flags
}

Server::~Server() {
    close(client_port_fd);//close client connection
    close(open_port_fd);//closes open port
    delete buffer;
}


void Server::send_msg(string s) {
    if (s.size()>SOCKET_BUF_SIZE)
        cout<<"ERROR MESSAGE SIZE TOO BIG\n";
    else{
        if(send(client_port_fd, s.c_str(), s.size(), 0)<0)//0 means no flags
            cout<<"ERROR SENDING MESSAGE\n";
    }

}

string Server::read_msg() {
    string msg;
    if(recv(client_port_fd, buffer, SOCKET_BUF_SIZE,0)<0)
        cout<<"ERROR READING MESSAGE\n";
    else{
        msg= string(buffer);

        //remove newline chars, mac, linux, and windows
        char chars[] = {'\r', '\n'};

        for (unsigned int i = 0; i < strlen(chars); ++i) {
			msg.erase(std::remove(msg.begin(), msg.end(), chars[i]), msg.end());
		}

        //msg.replace(msg.end()-2,msg.end(),"");//replaces the newline char and return char with nothing
        memset(buffer,0,SOCKET_BUF_SIZE);//clear buffer
        cout << "RESPONSE " << msg << "\n";
        return msg;
        }
    return string("error");
}

int Server::operator << (string& s) { //int is an error number, operator calls send_msg
    int n=0;
    if (s.size()>SOCKET_BUF_SIZE)
        cout<<"ERROR MESSAGE SIZE TOO BIG\n";
    else if((n=send(client_port_fd, s.c_str(), s.size(), 0))<0)//0 means no flags
            cout<<"ERROR SENDING MESSAGE\n";
    return n;
}

int Server::operator >> (string& s) { //int is an error number, operator calls read_msg
    int n=0;
    if((n=recv(client_port_fd, buffer, SOCKET_BUF_SIZE,0))<0)
        cout<<"ERROR READING MESSAGE\n";
    return n;

    }
