#include "SpaceInvaders.h"
#include <iostream>
using namespace invaders;
using namespace std;

int main() {
	SpaceInvaders game;
	if (!game.valid()) {
		cout << "Failed to create Box2D world" << endl;
		return 1;
	}
	game.run();
	return 0;
}
