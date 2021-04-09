#include <stdio.h>

#include "CPU.h"

int main() {
	CPU cpu;

	do {
		cpu.clock();
	} while (cpu.cycles != 0);

	return 0;
}
