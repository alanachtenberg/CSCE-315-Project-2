#include <iostream>

class Cell {
public:
	enum DIRECTION { N, NE, E, SE, S, SW, W, NW };
	enum STATE { EMPTY, BLACK, WHITE };
private:
	STATE state;
	int x, y;
public:
	Cell() {
		state = EMPTY;
		x = 0;
		y = 0;
	}

	void setState(STATE s) { state = s; }
	void setPosition(int x_pos, int y_pos) { x = x_pos; y = y_pos; }
	STATE getState() { return state; }
	int getX() { return x; }
	int getY() { return y; }

	friend std::ostream& operator<<(std::ostream &out, Cell &cell);
};
