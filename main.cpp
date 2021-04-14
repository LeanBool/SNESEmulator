#include <stdio.h>
#include "Debugging/Debugger.h"
#include "CPU.h"
#include <iostream>

void testProgramm(CPU& cpu) // WORKS
{
	//0000        INX             E8
	//0001        INX             E8
	//0002        NOP             EA
	//0003        CPX #$06        C9 06
	//0005        BNE $00         D0 F9
	std::vector<int> prog = { 0xE8, 0xE8, 0xEA, 0xE0, 0x06, 0xD0, 0xF9 };
	for (int i = 0; i < prog.size(); i++)
	{
		cpu.write(i, prog.at(i));
	}
	while (cpu.PC < 8)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			cpu.clock();
		}
	}
}
void testProgamm2(CPU& cpu) // WORKS
{
	//0000        LDA #$01        A9 01
	//0002        STA $0200       8D 00 02
	//0005        LDA #$05        A9 05
	//0007        STA $0201       8D 01 02
	//000A        LDA #$08        A9 08
	//000C        STA $0202       8D 02 02
	std::vector<int> prog = { 0xA9, 0x01, 0x8D, 0x00, 0x02, 0xA9, 0x05, 0x8D, 0x01, 0x02, 0xA9, 0x08, 0x8D, 0x02, 0x02 };
	for (int i = 0; i < prog.size(); i++)
	{
		cpu.write(i, prog.at(i));
	}
	while (cpu.PC < 0x10)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			cpu.clock();
		}
	}
}
void testProgramm3(CPU& cpu)
{
	//0000        LDX #$01        A2 01
	//0002        LDA #$05        A9 05
	//0004        STA $0001       8D 01 00
	//0007        LDA #$07        A9 07
	//0009        STA $0002       8D 02 00
	//000C        LDY #$0A        A0 0A
	//000E        STY $0705       8C 05 07
	//0011        LDA($00, X)     A1 00
	std::vector<int> prog = { 0xA2, 0x01, 0xA9, 0x05, 0x8D, 0x01, 0x00, 0xA9, 0x07, 0x8D, 0x02, 0x00, 0xA0, 0x0A, 0x8C, 0x05, 0x07, 0xA1, 0x00};
	for (int i = 0; i < prog.size(); i++)
	{
		cpu.write(i, prog.at(i));
	}
	while (cpu.PC < 0x15)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			cpu.clock();
		}
	}
}
void testBlockMove(CPU& cpu) {
	//LDA #4
	//LDX #0
	//LDY #0xB
	//MVN 0, 0
	//NOP
	//NOP

	std::vector<int> prog = { 0xA9, 0x04, 0xA2, 0x00, 0xA0, 0x0B, 0x54, 0x00, 0x00, 0xEA, 0xEA };
	for (int i = 0; i < prog.size(); i++) {
		cpu.write(i, prog.at(i));
	}
	while (cpu.PC < 0x9) {
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			cpu.clock();
		}
	}
}

int main() {
	CPU cpu;
	static DebugWindow debWind(cpu);
	int16_t operand = 0;

	//testProgramm(cpu);
	//testProgamm2(cpu);
	//testProgramm3(cpu);
	testBlockMove(cpu);

	while (Window::Activ())
	{
	
	}

	return 0;
}
