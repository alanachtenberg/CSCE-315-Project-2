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
			b.setCell(8, 8, Cell::BLACK);

			//N = 1

			//NW = 2
			b.setCell(9, 7, Cell::BLACK);

			//E = 3
			b.setCell(9, 8, Cell::BLACK);
			b.setCell(10, 8, Cell::BLACK);

			//SE = 4
			b.setCell(9, 9, Cell::BLACK);
			b.setCell(10, 10, Cell::BLACK);
			b.setCell(11, 11, Cell::BLACK);

			//S = 5
			b.setCell(8, 9, Cell::BLACK);
			b.setCell(8, 10, Cell::BLACK);
			b.setCell(8, 11, Cell::BLACK);
			b.setCell(8, 12, Cell::BLACK);

			//SW = 6
			b.setCell(7, 9, Cell::BLACK);
			b.setCell(6, 10, Cell::BLACK);
			b.setCell(5, 11, Cell::BLACK);
			b.setCell(4, 12, Cell::BLACK);
			b.setCell(3, 13, Cell::BLACK);

			//W = 7
			b.setCell(7, 8, Cell::BLACK);
			b.setCell(6, 8, Cell::BLACK);
			b.setCell(5, 8, Cell::BLACK);
			b.setCell(4, 8, Cell::BLACK);
			b.setCell(3, 8, Cell::BLACK);
			b.setCell(2, 8, Cell::BLACK);

			//NW = 8
			b.setCell(7, 7, Cell::BLACK);
			b.setCell(6, 6, Cell::BLACK);
			b.setCell(5, 5, Cell::BLACK);
			b.setCell(4, 4, Cell::BLACK);
			b.setCell(3, 3, Cell::BLACK);
			b.setCell(2, 2, Cell::BLACK);
			b.setCell(1, 1, Cell::BLACK);
			//b.setCell(1, 1, Cell::WHITE);
			b.setCell(15, 15, Cell::EMPTY);

		}
		catch(runtime_error e) {
			cerr << e.what() << "\n";
		}

		Cell center = b.getCell(8, 0xf);

		cout << b << "\n";
		cout << "N:  " << b.checkPath(center, Cell::N)  << "\n";
		cout << "NE: " << b.checkPath(center, Cell::NE) << "\n";
		cout << "E:  " << b.checkPath(center, Cell::E)  << "\n";
		cout << "SE: " << b.checkPath(center, Cell::SE) << "\n";
		cout << "S:  " << b.checkPath(center, Cell::S)  << "\n";
		cout << "SW: " << b.checkPath(center, Cell::SW) << "\n";
		cout << "W:  " << b.checkPath(center, Cell::W)  << "\n";
		cout << "NW: " << b.checkPath(center, Cell::NW) << "\n";
}
