class Player {
public:
	enum MODE { HUMAN, AI };
	enum DIFFICULTY { EASY, MEDIUM, HARD, RANDOM };

private:
	std::string name;
	int color;

public:
	Player() {

	}

	friend std::ostream& operator<<(std::ostream &out, const Player &player);
};
