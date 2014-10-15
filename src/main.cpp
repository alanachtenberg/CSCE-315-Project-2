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
			b.setCell(8, 8, Cell::BLACK);
			cout << b << "\n";
			b.setCell(8, 9, Cell::BLACK);
			cout << b << "\n";
			b.setCell(8, 10, Cell::BLACK);
			cout << b << "\n";
			b.setCell(9, 11, Cell::BLACK);
			cout << b << "\n";
			b.setCell(10, 11, Cell::BLACK);
			cout << b << "\n";
			b.setCell(11, 11, Cell::BLACK);
			cout << b << "\n";

			b.setCell(8, 11, Cell::BLACK);
			cout << b << "\n";
		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}
}
