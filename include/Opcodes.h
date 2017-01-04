#ifndef OPCODES_H
#define OPCODES_H

void (*chip8_lookup[16])();
void (*chip8_arithmetic[16])();	

void op_0xxx();
void op_1xxx();
void op_2xxx();
void op_3xxx();
void op_4xxx();
void op_5xxx();
void op_6xxx();
void op_7xxx();
void op_8xxx();
void op_9xxx();
void op_Axxx();
void op_Bxxx();
void op_Cxxx();
void op_Dxxx();
void op_Exxx();
void op_Fxxx();

void op_xxx0();
void op_xxx1();
void op_xxx2();
void op_xxx3();
void op_xxx4();
void op_xxx5();
void op_xxx6();
void op_xxx7();
void op_xxxE();

#endif
