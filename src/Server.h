
#include <sys/socket.h> //includes for POSIX socket
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // include for string
#include <unistd.h>
#include <errno.h>
#include <string>
#include <sys/types.h>
#include <time.h>

using namespace std;
class Server {

    private:
        int         open_port_fd; //file descriptor of port that will be set to listen
        int         client_port_fd; //file descriptor of the client, read and write functions
        sockaddr_in server_addr; //address information of server, will set this to IPV4 adrress
        char        buffer[4096]; //contains the write and read strings of the socket
    public:

        Server(int port_num);   //initialize socket
        //string must be smaller than 4K bits
        void send_msg(string s);    //sends message to client
        string read_msg();    //reads message from client

        int operator << (string& s); //int is an error number, operator calls send_msg
        int operator >> (string& s); //int is an error number, operator calls read_msg
};
