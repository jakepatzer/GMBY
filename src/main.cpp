#include <SDL.h>
#include "CPU.h"
#include "Definitions.h"
#include "Registers.h"
#include<iostream>
using namespace std;

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

int main(int argc, char* argv[]) {
	
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else {
		window = SDL_CreateWindow("Game Boy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {

			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		}
	}

	cout << "ROM file name: ";
	string game;
	cin >> game;

	CPU gameboy(window, renderer);
	gameboy.loadGame("roms//" + game);
	gameboy.run();

	return 0;
}
