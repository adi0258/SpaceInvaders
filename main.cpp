#include "SpaceInvaders.h"
#include <iostream>
using namespace invaders;
using namespace std;

void run_sprite_atlas_preview_test();

int main() {
	run_sprite_atlas_preview_test();
	SpaceInvaders game;
	if (!game.valid()) {
		cout << "Failed to create Box2D world" << endl;
		return 1;
	}
	game.run();
	return 0;
}
