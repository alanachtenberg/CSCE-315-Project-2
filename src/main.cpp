#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include "Board.h"
#include "Server.h"
using namespace std;

int main(int argc, char** argv) {
		Board b;
		try {
			//base
            Server myServer(4300);
            string mesg=myServer.read_msg();
            cout<<mesg<<endl;
			cout << b << "\n";
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
