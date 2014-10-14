#include <iostream>
#include <exception>
#include "Board.h"

using namespace std;

int main(int argc, char** argv) {
		Board b;
		try {
			b.setCell(2, 2, Cell::WHITE);
			b.setCell(2, 3, Cell::BLACK);
			b.setCell(3, 2, Cell::WHITE);
		}
		catch(exception e) {
			cerr << e.what() << "\n";
		}

		cout << b << "\n" ;

}
