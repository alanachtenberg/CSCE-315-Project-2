#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <unistd.h>
#include "Board.h"

using namespace std;

int main(int argc, char** argv) {
	Board b;
	b.shell();

	//~ Timer timer;
	//~ timer.start();
	//~ usleep(60000001);
	//~ timer.finish();
	//~ cout << "\n" << timer << "\n";

	return 0;
}
