#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Opcodes.h"
#include "chip.h"

void (*chip8_lookup[16])() =
{
	op_0xxx,
	op_1xxx,
	op_2xxx,
	op_3xxx,
	op_4xxx,
	op_5xxx,
	op_6xxx,
	op_7xxx,
	op_8xxx,
	op_9xxx,
	op_Axxx,
	op_Bxxx,
	op_Cxxx,
	op_Dxxx,
	op_Exxx,
	op_Fxxx
};

void (*chip8_arithmetic[16])() =
{
	op_xxx0,
	op_xxx1,
	op_xxx2,
	op_xxx3,
	op_xxx4,
	op_xxx5,
	op_xxx6,
	op_xxx7,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	op_xxxE,
	NULL
};

void op_0xxx() 
{
	if ((Chip8.Opcode & 0x00FF) == 0x00E0)
	{
		memset(Chip8.Disp_Mem, 0, sizeof(Chip8.Disp_Mem));
		Chip8.Pc += 2;
	}
	else if ((Chip8.Opcode & 0x00FF) == 0x00EE)
	{
		Chip8.Pc = Chip8.Stack[(--Chip8.Sp)&0xF] + 2;
	} 
	else
	{
		printf("ERROR: Unknown Opcode %X\n", Chip8.Opcode);
	}

}

void op_1xxx()
{
	Chip8.Pc = Chip8.Opcode & 0x0FFF;
}

void op_2xxx()
{
	if((Chip8.Sp + 1) >= sizeof(Chip8.Stack)) 
	{
		printf("Stack Overflow\n");
		exit(1);
	}
	Chip8.Stack[Chip8.Sp] = Chip8.Pc;
	Chip8.Sp += 1;
	Chip8.Pc = Chip8.Opcode & 0x0FFF;
}

void op_3xxx()
{
	if(Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] == (Chip8.Opcode & 0x00FF))
		Chip8.Pc += 4;
	else
		Chip8.Pc += 2;
}

void op_4xxx()
{
	if(Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] != (Chip8.Opcode & 0x00FF))
		Chip8.Pc += 4;
	else
		Chip8.Pc += 2;
}

void op_5xxx()
{
	if(Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] == Chip8.V[(Chip8.Opcode & 0x00F0) >> 4])
		Chip8.Pc += 4;
	else
		Chip8.Pc += 2;
}

void op_6xxx()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.Opcode & 0x00FF;
	Chip8.Pc += 2;
}

void op_7xxx()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] += Chip8.Opcode & 0x00FF;
	Chip8.Pc += 2;
}

void op_8xxx()
{
	chip8_arithmetic[Chip8.Opcode & 0x000F]();
}

void op_9xxx()
{
	if(Chip8.V[(Chip8.Opcode & 0xF00) >> 8] != Chip8.V[(Chip8.Opcode & 0x00F0) >> 4])
	{
		Chip8.Pc += 4;
	} else {
		Chip8.Pc += 2;
	}
}

void op_Axxx()
{
	Chip8.I = Chip8.Opcode & 0x0FFF;
	Chip8.Pc += 2;
}

void op_Bxxx()
{
	Chip8.Pc = (Chip8.Opcode & 0x0FFF) + Chip8.V[0];
}

void op_Cxxx()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = (rand() % 256) & (Chip8.Opcode & 0x00FF);
	Chip8.Pc += 2;
}

void op_Dxxx()
{
	uint16_t x = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8];
	uint16_t y = Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	uint16_t height = Chip8.Opcode & 0x000F;
	uint16_t pixel;

	Chip8.V[0xF] = 0;
	for (int yline = 0; yline < height; yline++)
	{
		pixel = Mem[Chip8.I + yline];
		for(int xline = 0; xline < 8; xline++)
		{
			if(pixel & (0x80 >> xline)) 
			{
				if(Chip8.Disp_Mem[(x + xline) + ((y + yline) * 64)])
				       Chip8.V[0xF] = 1;
				Chip8.Disp_Mem[x + xline + ((y + yline) * 64)] ^=1;
			}
		}
	}
	Chip8.DFlag = 1;
	Chip8.Pc += 2;	
}

void op_Exxx()
{
	if((Chip8.Opcode & 0x000F) == 0x000E)
	{
		if(Chip8.Key[Chip8.V[(Chip8.Opcode & 0x0F00) >> 8]])
			Chip8.Pc += 4;
		else
			Chip8.Pc += 2;
	}
	else if ((Chip8.Opcode & 0x000F) == 0x0001)
	{
		if (!Chip8.Key[Chip8.V[(Chip8.Opcode & 0x0F00) >> 8]])
			Chip8.Pc += 4;
		else
			Chip8.Pc += 2;
	}
	else
	{
		printf("Wrong opcode: %X\n", Chip8.Opcode);
		getchar();
	}
}

void op_Fxxx()
{
	if((Chip8.Opcode & 0x00FF) == 0x0007)
	{
		Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.Delay_Timer;
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x000A)
	{
		for(int i = 0; i < 16; i++)
		{
			if(Chip8.Key[i] != 0)
			{
				Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.Key[i];
				Chip8.Pc = Chip8.Pc + 2;
				break;
			}
		}
	}
	else if((Chip8.Opcode & 0x00FF) == 0x0015)
	{
		Chip8.Delay_Timer = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8];
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x0018)
	{
		Chip8.Sound_Timer = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8];
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x001E)
	{
		Chip8.I += Chip8.V[(Chip8.Opcode & 0x0F00) >> 8];
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x0029)
	{
		Chip8.I = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] * 5;
		Chip8.Pc += 2;
	}
	else if ((Chip8.Opcode & 0x00FF) == 0x0033)
	{
		Mem[Chip8.I] = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] / 100;
		Mem[Chip8.I+1] = (Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] /10) % 10;
		Mem[Chip8.I+2] = (Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] % 100) % 10;
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x0055)
	{
		for(int i = 0; i <= (Chip8.Opcode & 0x0F00) >> 8; i++)
		{
			Mem[Chip8.I+i] = Chip8.V[i];
		}
		Chip8.Pc += 2;
	}
	else if((Chip8.Opcode & 0x00FF) == 0x0065)
	{
		for(int i = 0; i <= (Chip8.Opcode & 0x0F00) >> 8; i++)
		{
			Chip8.V[i] = Mem[Chip8.I+i];
		}
		Chip8.Pc += 2;
	}
}

void op_xxx0()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx1()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] |= Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx2()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] &= Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx3()
{
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] ^= Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx4()
{
	if((Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] + Chip8.V[(Chip8.Opcode & 0x00F0) >> 4]) > 255)
		Chip8.V[0xF] = 1;
	else
		Chip8.V[0xF] = 0;
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] + Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx5()
{
	if(Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] > Chip8.V[(Chip8.Opcode & 0x00F0) >> 4])
	{
		Chip8.V[0xF] = 1;
	} else {
		Chip8.V[0xF] = 0;
	}
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] - Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxx6()
{
	Chip8.V[0xF] = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] & 7;
	Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] >>= 1;
	Chip8.Pc += 2;
}

void op_xxx7()
{
	Chip8.V[0xF] = Chip8.V[(Chip8.Opcode & 0x00F0) >> 4] <= Chip8.V[(Chip8.Opcode & 0x0F00) >> 8];
	Chip8.V[(Chip8.Opcode & 0x00F0) >> 4] = Chip8.V[(Chip8.Opcode & 0x0F00) >> 8] - Chip8.V[(Chip8.Opcode & 0x00F0) >> 4];
	Chip8.Pc += 2;
}

void op_xxxE()
{
	Chip8.V[0xF] = Chip8.V[(Chip8.Opcode & 0x00F0)] >> 7;
	Chip8.V[(Chip8.Opcode & 0x00F0) >> 4] <<= 1;
	Chip8.Pc += 2;
}
