#pragma once
#include <stdint.h>
#include <vector>
#include <string>

class CPU
{
public:
	CPU();
	~CPU();

public:
	enum flags {
		C = (1 << 0),
		Z = (1 << 1),
		I = (1 << 2),
		D = (1 << 3),
		INDEX = (1 << 4),
		M = (1 << 5),
		V = (1 << 6),
		N = (1 << 8),
	};

	std::vector<std::vector<uint8_t>> memory;

	uint16_t A = 0x0;
	uint16_t X = 0x0;
	uint16_t Y = 0x0;
	uint16_t SP = 0x0;
	uint8_t DBR = 0x0;
	uint16_t DP = 0x0;
	uint8_t PBR = 0x0;
	uint16_t PC = 0x0;
	uint8_t status = 0x0;

	uint8_t opcode = 0x0;
	uint8_t cycles = 0x0;
	uint8_t fetched = 0x0;
	uint32_t address_absolute = 0x0;
	uint32_t address_relative = 0x0;

	//Addressing modes
	uint8_t ABS(); // absolute
	uint8_t ABX(); // absolute, x
	uint8_t ABY(); // absolute, y
	uint8_t AII(); // absolute indexed indirect
	uint8_t ABI(); // absolute indirect
	uint8_t ABL(); // absolute long
	uint8_t ABLX();  // absolute long indexed
	uint8_t ACC(); // accumulator
	uint8_t BLM(); // block move
	uint8_t DIR(); // direct 
	uint8_t DIN(); // direct indirect
	uint8_t DIX(); // direct indirect indexed
	uint8_t DIXL(); // direct indirect indexed long
	uint8_t DIL(); // direct indirect long
	uint8_t DXI(); // direct indexed indirect
	uint8_t DIRX(); // direct, x
	uint8_t DIRY(); // direct, y
	uint8_t IMM(); // immediate
	uint8_t IMP(); // implied
	uint8_t REL(); // relative
	uint8_t RELL(); // relative long
	uint8_t STK(); // stack
	uint8_t SREL(); // stack relative
	uint8_t SRII(); // stack relative indirect indexed

	//Opcodes
	uint8_t ADC(); uint8_t AND(); uint8_t ASL(); uint8_t BCC(); uint8_t BCS(); uint8_t BEQ();
	uint8_t BIT(); uint8_t BMI(); uint8_t BNE(); uint8_t BPL(); uint8_t BRA(); uint8_t BRK();
	uint8_t BRL(); uint8_t BVC(); uint8_t BVS(); uint8_t CLC(); uint8_t CLD(); uint8_t CLI();
	uint8_t CLV(); uint8_t CMP(); uint8_t CPX(); uint8_t CPY(); uint8_t COP(); uint8_t DEC();
	uint8_t DEX(); uint8_t DEY(); uint8_t EOR(); uint8_t INC(); uint8_t INX(); uint8_t INY();
	uint8_t JMP(); uint8_t JML(); uint8_t JSR(); uint8_t JSL(); uint8_t LDA(); uint8_t LDX();
	uint8_t LDY(); uint8_t LSR(); uint8_t MVN(); uint8_t MVP(); uint8_t NOP(); uint8_t ORA();
	uint8_t PEA(); uint8_t PEI(); uint8_t PER(); uint8_t PHA(); uint8_t PHB(); uint8_t PHD(); 
	uint8_t PHK(); uint8_t PHP(); uint8_t PHX(); uint8_t PHY(); uint8_t PLA(); uint8_t PLB(); 
	uint8_t PLD(); uint8_t PLP(); uint8_t PLX(); uint8_t PLY(); uint8_t REP(); uint8_t ROL(); 
	uint8_t ROR(); uint8_t RTI(); uint8_t RTS(); uint8_t RTL(); uint8_t SBC(); uint8_t SEC();
	uint8_t SED(); uint8_t SEI(); uint8_t SEP(); uint8_t STA(); uint8_t STX(); uint8_t STY(); 
	uint8_t STP(); uint8_t STZ(); uint8_t TAX(); uint8_t TAY(); uint8_t TCD(); uint8_t TCS();
	uint8_t TDC(); uint8_t TSC(); uint8_t TSX(); uint8_t TXA(); uint8_t TXS(); uint8_t TXY();
	uint8_t TYA(); uint8_t TYX(); uint8_t TRB(); uint8_t TSB(); uint8_t WAI(); uint8_t WDM(); 
	uint8_t XBA(); uint8_t XCE();

	void clock();
	uint8_t fetch();

	uint8_t read(uint32_t address);
	void write(uint32_t address, uint8_t data);

	void SetFlag(flags f, bool v);
	uint16_t GetFlag(flags f);

private:
	struct INSTRUCTION {
		std::string name;
		uint8_t(CPU::* operate)(void) = nullptr;
		uint8_t(CPU::* address_mode)(void) = nullptr;
		uint8_t cycles = 0;
	};

	std::vector<INSTRUCTION> lookup;
};

