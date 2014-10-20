#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include "Board.h"
#include "Server.h"
using namespace std;

int main(int argc, char** argv) {
		Board b;
		try {
			//base
            Server myServer(2323);
            //string mesg=myServer.read_msg();
           // cout<<mesg<<endl;
			cout << b << "\n";
			return 0;
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
