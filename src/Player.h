class Player {
	std::string name;
	int color;

public:
	Player() {

	}

	friend std::ostream& operator<<(std::ostream &out, const Player &player);
};
