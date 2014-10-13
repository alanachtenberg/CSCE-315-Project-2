#include <iostream>

class Cell {
public:

	enum SEARCH_DIRECTION { N, NE, E, SE, S, SW, W, NW };
	enum STATE { EMPTY, BLACK, WHITE };
	Cell() {

	}

	friend std::ostream& operator<<(std::ostream &out, const Cell &cell);
};
