main: main.cpp Board.o Player.o Cell.o Server.o
     g++ -std=c++11 main.cpp Board.o Player.o Cell.o

Board.o: Board.cpp Board.h Cell.h Player.h Server.h
	g++ -std=c++11 -c Board.cpp

Cell.o: Cell.cpp Cell.h
	g++ -std=c++11 -c Cell.cpp

Player.o: Player.cpp Player.h
	g++ -std=c++11 -c Player.cpp

Server.o: Server.cpp Server.h
	g++ -std=c++11 -c Server.cpp

clean:
	rm main
	rm *.o

run: main
	./main

run_file_output: main
	./main > output.txt
