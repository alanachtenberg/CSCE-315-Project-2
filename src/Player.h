class Player {
public:
	enum MODE { HUMAN, AI };
	enum DIFFICULTY { EASY, MEDIUM, HARD, RANDOM };

private:

public:
	Player() {

	}

	friend std::ostream& operator<<(std::ostream &out, const Player &player);
};
