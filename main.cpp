#include <stdio.h>

#include "CPU.h"

int main() {
	CPU cpu;
	cpu.write(0x0, 0x0);
	cpu.write(0x1, 0xFF);
	cpu.write(0x2, 0x30);
	cpu.write(0x3, 0x00);

	cpu.write(0x30FF, 0x5);

	do {
		cpu.clock();
	} while (cpu.cycles != 0);

	printf_s("%08X\n", cpu.read(0x30FF));
	printf_s("%d\n", cpu.A);
	return 0;
}
