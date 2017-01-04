#ifndef _CHIP_
#define _CHIP_

#include<stdint.h>
#include<stdio.h>

extern unsigned char Mem[0x1000];
extern uint8_t chip8_fontset[80];

struct Cpu
{
	FILE *ptr;
	uint16_t Opcode;		
	uint8_t V[0x10];	
	uint16_t I;			
	uint8_t Delay_Timer;
	uint8_t Sound_Timer;
	uint16_t Pc;
	uint8_t Sp;
	uint16_t Stack[0x10];
	uint8_t Key[0x10];
	uint8_t Disp_Mem[64*32];
	uint32_t DFlag;
};

extern struct Cpu Chip8;

void chip8_init();
void chip8_load();
void chip8_timers();
void chip8_emulate();

#endif
