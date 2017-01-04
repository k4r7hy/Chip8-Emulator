#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "chip.h"
#include "Opcodes.h"

struct Cpu Chip8;

unsigned char Mem[0x1000];

uint8_t chip8_fontset[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,
	0x20, 0x60, 0x20, 0x20, 0x70,
	0xF0, 0x10, 0xF0, 0x80, 0xF0,
	0xF0, 0x10, 0xF0, 0x10, 0xF0,
	0x90, 0x90, 0xF0, 0x10, 0x10,
	0xF0, 0x80, 0xF0, 0x10, 0xF0,
	0xF0, 0x80, 0xF0, 0x90, 0xF0,
	0xF0, 0x10, 0x20, 0x40, 0x40,
	0xF0, 0x90, 0xF0, 0x90, 0xF0,
	0xF0, 0x90, 0xF0, 0x10, 0xF0,
	0xF0, 0x90, 0xF0, 0x90, 0x90,
	0xE0, 0x90, 0xE0, 0x90, 0xE0,
	0xF0, 0x80, 0x80, 0x80, 0xF0,
	0xE0, 0x90, 0x90, 0x90, 0xE0,
	0xF0, 0x80, 0xF0, 0x80, 0xF0,
	0xF0, 0x80, 0xF0, 0x80, 0x80
};

void chip8_init(){
	
	int i;
	
	Chip8.DFlag = 0;
	srand(time(NULL));
 	Chip8.Pc = 0x200;
	Chip8.Opcode = 0x00;
	Chip8.I = 0x00;
	Chip8.Sp = 0x00;
	Chip8.Delay_Timer = 0;
	Chip8.Sound_Timer = 0;

	for(i = 0; i < 0x10; i++) {
		Chip8.V[i] = 0;
		Chip8.Stack[i] = 0;
		Chip8.Key[i] = 0;
	}

	for(i = 0; i < 0x100; i++) {
		Chip8.Disp_Mem[i] = 0;
	}
}
void chip8_load() {
	int i;
	char file[100];
	int c;
	
	for(int n = 0; n < 100; n++){
		file[n] = 0;
	}
	
	printf("Enter game name:");
	
	for(i = 0; i < 99 && (c = getchar()) != EOF && c != '\n'; ++i) {
		file[i] = c;
	}
	if(c == '\n'){
		file[i] = '\0';
	}
	
	for(i = 0; i<80; i++){
		Mem[i] = chip8_fontset[i];
	}
	
	Chip8.ptr = fopen(file, "rb");
	if(Chip8.ptr == NULL){
		printf("Incompatable File\n");
		exit(1);
	}
	int n = 0;
	do {
		c = fgetc(Chip8.ptr);
		Mem[0x0200+n++] = c;
		}while (c != EOF);
	fclose(Chip8.ptr);

//	for(i = 0; *Chip8.ptr!=EOF; fseek(Chip8.ptr, i, SEEK_SET),i++){
//	       Mem[0x200+i] = *Chip8.ptr;
//	}	
	
//	fread(Mem+0x200, 1, 0x1000-0x200, Chip8.ptr);       
}

void chip8_timers() {
	if(Chip8.Delay_Timer > 0)
		Chip8.Delay_Timer--;
	if(Chip8.Sound_Timer > 0) {
		if(Chip8.Sound_Timer == 1)
			;//IMPLEMENT BEEP USING SDL
		Chip8.Sound_Timer--;
	}
}		

void chip8_emulate() {
	Chip8.Opcode = Mem[Chip8.Pc] << 8 | Mem[Chip8.Pc + 1];
	chip8_lookup[(Chip8.Opcode & 0xF000) >> 12]();
	chip8_timers();
}


