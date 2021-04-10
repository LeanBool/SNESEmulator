#include <stdio.h>

#include "CPU.h"

bool testABS(CPU &cpu) {
	bool ret = false;
	printf("ABS Test:\n");

	std::vector<int> inst = { 0xAD, 0xFF, 0xFF };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.write(0xFFFF, 0x1);

	do {
		cpu.clock();
	} while (cpu.cycles != 0);

	if (cpu.A == 0x1) {
		ret = true;
	}

	cpu.clearMemory();
	cpu.clearRegisters();

	return ret;
}

bool testABX(CPU &cpu) {
	bool ret = false;
	printf("\nABX Test:\n");

	std::vector<int> inst = { 0xBD, 0xFE, 0xFF };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.write(0xFFFF, 0x1);
	cpu.X = 1;

	do {
		cpu.clock();
	} while (cpu.cycles != 0);

	if (cpu.A == 0x1) {
		ret = true;
	}

	cpu.clearMemory();
	cpu.clearRegisters();

	return ret;
}

bool testABY(CPU& cpu) {
	bool ret = false;
	printf("\nABY Test:\n");

	std::vector<int> inst = { 0xB9, 0xFE, 0xFF };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.write(0xFFFF, 0x1);
	cpu.Y = 1;

	do {
		cpu.clock();
	} while (cpu.cycles != 0);

	if (cpu.A == 0x1) {
		ret = true;
	}

	cpu.clearMemory();
	cpu.clearRegisters();

	return ret;
}

bool testAII(CPU& cpu) {
	bool ret = false;
	printf("\nAII Test:\n");

	std::vector<int> inst = { 0x7C, 0x00, 0xFF, 0xEA };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.X = 0xFE;
	cpu.write(0xFFFE, 0x3);
	cpu.write(0xFFFF, 0x0);

	do {
		cpu.clock();
	} while (cpu.opcode != 0xEA || cpu.cycles != 0);

	cpu.clearMemory();
	cpu.clearRegisters();

	return 1;
}

bool testABI(CPU& cpu) {
	bool ret = false;
	printf("\nABI Test:\n");

	std::vector<int> inst = { 0x6C, 0xFE, 0xFF, 0xEA };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.write(0xFFFE, 0x3);
	cpu.write(0xFFFF, 0x0);

	do {
		cpu.clock();
	} while (cpu.opcode != 0xEA || cpu.cycles != 0);

	cpu.clearMemory();
	cpu.clearRegisters();

	return 1;
}

bool testABL(CPU& cpu) {
	bool ret = false;
	printf("\nABL Test:\n");

	std::vector<int> inst = { 0xAF, 0xFE, 0x00, 0x02, 0xEA };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}
	cpu.write(0x0200FE, 0xFF);
	cpu.write(0x0200FF, 0x10);

	do {
		cpu.clock();
	} while (cpu.opcode != 0xEA || cpu.cycles != 0);

	ret = cpu.A == 0x10FF;

	cpu.clearMemory();
	cpu.clearRegisters();

	return ret;
}


bool testIMM(CPU& cpu) {
	bool ret = false;
	printf("\nIMM Test:\n");

	std::vector<int> inst = { 0xA9, 0x00, 0x10, 0xEA };
	for (int i = 0; i < inst.size(); i++) {
		cpu.write(i, inst[i]);
	}

	do {
		cpu.clock();
	} while (cpu.opcode != 0xEA || cpu.cycles != 0);

	ret = cpu.A == 0x1000;

	cpu.clearMemory();
	cpu.clearRegisters();

	return ret;
}

int main() {
	CPU cpu;

	bool abs = testABS(cpu);
	bool abx = testABX(cpu);
	bool aby = testABY(cpu);
	bool aii = testAII(cpu);
	bool abi = testABI(cpu);
	bool abl = testABL(cpu);
	bool imm = testIMM(cpu);

	printf("\nAbsolute addressing test: %d\n", abs);
	printf("Absolute X addressing test: %d\n", abx);
	printf("Absolute Y addressing test: %d\n", aby);
	printf("Absolute indexed indirect addressing test: %d\n", aii);
	printf("Absolute indirect adressing test: %d\n", abi);
	printf("Absolute long addressing test: %d\n", abl);
	printf("Immmediate addressing test: %d\n", imm);

	return 0;
}
