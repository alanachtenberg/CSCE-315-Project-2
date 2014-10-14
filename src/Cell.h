#include <iostream>

class Cell {
public:
	enum SEARCH_DIRECTION { N, NE, E, SE, S, SW, W, NW };
	enum STATE { EMPTY, BLACK, WHITE };
private:
	STATE state;
public:
	Cell() {
		state = EMPTY;
	}

	void setState(STATE s) { state = s; }
	STATE getState() { return state; }

	friend std::ostream& operator<<(std::ostream &out, Cell &cell);
};
