#include "SDL/SDL.h"
#include <unistd.h>

#include "SDL_.h"
#include "Opcodes.h"
#include "chip.h"

int main() {
	chip8_init();	
	chip8_load();
	SDL_Init(SDL_INIT_EVERYTHING);
	//Set display window to 640x320 Res and 32 bits-per-pixel
	//SDL_HWSURFACE: Create video surface in video memory
	//SDL_DOUBLEBUF: Enable hardware double buffering
	SDL_SetVideoMode(640, 320, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	for(;;) {
		chip8_emulate();
		usleep(500);
		if(Chip8.DFlag) {		
			Chip8.DFlag = 0;
			chip8_draw();
		}
		chip8_input();
	}
	return 0;
}
