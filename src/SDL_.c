#include "SDL/SDL.h"

#include "SDL_.h"
#include "chip.h"

void chip8_draw() {
	
	int i,j;
	//SDL_Surface basically its a structure which contains a collection of pixels
	//Used in software blitting
	SDL_Surface *surface = SDL_GetVideoSurface();
	SDL_LockSurface(surface);
	//Convert pixels to 32 bit
	Uint32 *screen = (Uint32 *)surface->pixels;
	memset(screen, 0, surface->w*surface->h*sizeof(Uint32));
	//This loop actually draws stuff to the screen, Display array is 64*32 in size and
	//sdl window is 640*320 [(j/10) + (i/10)*64] this part takes care of that
	for(i = 0; i < 320; i++) {
		for(j = 0; j < 640; j++) {
			screen[j+i*surface->w] = Chip8.Disp_Mem[(j/10) + (i/10)*64]?0xb2b2b2:0x4e534f;
		}
	}
	//SDL_UnlockSurface Unlocks the surface
	//Window is using double buffering, SDL_Flip swaps back buffer with front buffer
	SDL_UnlockSurface(surface);
	SDL_Flip(surface);
	//SDL_Delay(1000);
}

void chip8_input() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type)
	{
		case SDL_KEYUP:
			for(int i = 0; i<16; i++)
				Chip8.Key[i] = 0;
			break;
	}
	switch(event.key.keysym.sym)
	{
		case SDLK_q:
			exit(1);
			break;
		case SDLK_1:
			Chip8.Key[0] = 1;
			break;
		case SDLK_2:
			Chip8.Key[1] = 1;
			break;
		case SDLK_DOWN:
			Chip8.Key[2] = 1;
			break;
		case SDLK_4:
			Chip8.Key[3] = 1;
			break;
		case SDLK_LEFT:
			Chip8.Key[4] = 1;
			break;
		case SDLK_a:
			Chip8.Key[5] = 1;
			break;
		case SDLK_RIGHT:
			Chip8.Key[6] = 1;
			break;
		case SDLK_8:
			Chip8.Key[7] = 1;
			break;
		case SDLK_UP:
			Chip8.Key[8] = 1;
			break;
		case SDLK_m:
			Chip8.Key[9] = 1;
			break;
		case SDLK_0:
			Chip8.Key[10] = 1;
			break;
		case SDLK_b:
			Chip8.Key[11] = 1;
			break;
		case SDLK_c:
			Chip8.Key[12] = 1;
			break;
		case SDLK_d:
			Chip8.Key[13] = 1;
			break;
		case SDLK_e:
			Chip8.Key[14] = 1;
			break;
		case SDLK_f:
			Chip8.Key[15] = 1;
			break;
		default:
			break;

	}
}

