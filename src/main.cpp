#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include "Board.h"

using namespace std;

int main(int argc, char** argv) {
		Board b;
		try {
			//base

			cout << b << "\n";
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
