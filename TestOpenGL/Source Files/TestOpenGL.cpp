// TestOpenGL.cpp : Defines the entry point for the application.
//

#include "../Header Files/Game.h"

int main()
{
	Game game("Super videogame", 1920, 1080, 4, 5, false);
	
	while (!game.getWindowShouldClose()) {
		game.update();
		game.render();
	}

	return 0;
}
