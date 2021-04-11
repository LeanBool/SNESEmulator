#include "CPU.h"

std::vector<CPU::INSTRUCTION> CPU::lookup;
CPU::CPU()
{
    memory.resize(256);
    emulation_mode = true;
    for (int i = 0; i < 256; i++) {
        memory[i].resize(65536);
    }

    lookup = {
        {"BRK", &CPU::BRK, &CPU::STK, 7}, {"ORA", &CPU::ORA, &CPU::DXI, 6}, {"COP", &CPU::COP, &CPU::STK, 7}, {"ORA", &CPU::ORA, &CPU::SREL, 4}, {"TSB", &CPU::TSB, &CPU::DIR, 5}, {"ORA", &CPU::ORA, &CPU::DIR, 3}, {"ASL", &CPU::ASL, &CPU::DIR, 5}, {"ORA", &CPU::ORA, &CPU::DIL, 6}, {"PHP", &CPU::PHP, &CPU::STK, 3}, {"ORA", &CPU::ORA, &CPU::IMM, 2}, {"ASL", &CPU::ASL, &CPU::ACC, 2}, {"PHD", &CPU::PHD, &CPU::STK, 4}, {"TSB", &CPU::TSB, &CPU::ABS, 6}, {"ORA", &CPU::ORA, &CPU::ABS, 4}, {"ASL", &CPU::ASL, &CPU::ABS, 6}, {"ORA", &CPU::ORA, &CPU::ABL, 5},
        {"BPL", &CPU::BPL, &CPU::REL, 2}, {"ORA", &CPU::ORA, &CPU::DIX, 5}, {"ORA", &CPU::ORA, &CPU::DIN, 5}, {"ORA", &CPU::ORA, &CPU::SRII, 7}, {"TRB", &CPU::TRB, &CPU::DIR, 5}, {"ORA", &CPU::ORA, &CPU::DIRX, 4}, {"ASL", &CPU::ASL, &CPU::DIRX, 6}, {"ORA", &CPU::ORA, &CPU::DIXL, 6}, {"CLC", &CPU::CLC, &CPU::IMP, 2}, {"ORA", &CPU::ORA, &CPU::ABY, 4}, {"INC", &CPU::INC, &CPU::ACC, 2}, {"TCS", &CPU::TCS, &CPU::IMP, 2}, {"TRB", &CPU::TRB, &CPU::ABS, 6}, {"ORA", &CPU::ORA, &CPU::ABX, 4}, {"ASL", &CPU::ASL, &CPU::ABX, 7}, {"ORA", &CPU::ORA, &CPU::ABLX, 5},
        {"JSR", &CPU::JSR, &CPU::ABS, 6}, {"AND", &CPU::AND, &CPU::DXI, 6}, {"JSL", &CPU::JSL, &CPU::ABL, 8}, {"AND", &CPU::AND, &CPU::SREL, 4}, {"BIT", &CPU::BIT, &CPU::DIR, 3}, {"AND", &CPU::AND, &CPU::DIR, 3}, {"ROL", &CPU::ROL, &CPU::DIR, 5}, {"AND", &CPU::AND, &CPU::DIL, 6}, {"PLP", &CPU::PLP, &CPU::STK, 4}, {"AND", &CPU::AND, &CPU::IMM, 2}, {"ROL", &CPU::ROL, &CPU::ACC, 2}, {"PLD", &CPU::PLD, &CPU::STK, 5}, {"BIT", &CPU::BIT, &CPU::ABS, 4}, {"AND", &CPU::AND, &CPU::ABS, 4}, {"ROL", &CPU::ROL, &CPU::ABS, 6}, {"AND", &CPU::AND, &CPU::ABL, 5},
        {"BMI", &CPU::BMI, &CPU::REL, 2}, {"AND", &CPU::AND, &CPU::DIX, 5}, {"AND", &CPU::AND, &CPU::DIN, 5}, {"AND", &CPU::AND, &CPU::SRII, 7}, {"BIT", &CPU::BIT, &CPU::DIRX, 4}, {"AND", &CPU::AND, &CPU::DIRX, 4}, {"ROL", &CPU::ROL, &CPU::DIRX, 6}, {"AND", &CPU::AND, &CPU::DIXL, 6}, {"SEC", &CPU::SEC, &CPU::IMP, 2}, {"AND", &CPU::AND, &CPU::ABY, 4}, {"DEC", &CPU::DEC, &CPU::ACC, 2}, {"TSC", &CPU::TSC, &CPU::IMP, 2}, {"BIT", &CPU::BIT, &CPU::ABX, 4}, {"AND", &CPU::AND, &CPU::ABX, 4}, {"ROL", &CPU::ROL, &CPU::ABX, 7}, {"AND", &CPU::AND, &CPU::ABLX, 5},
        {"RTI", &CPU::RTI, &CPU::STK, 7}, {"EOR", &CPU::EOR, &CPU::DXI, 6}, {"WDM", &CPU::WDM, &CPU::IMP, 2}, {"EOR", &CPU::EOR, &CPU::SREL, 4}, {"MVP", &CPU::MVP, &CPU::BLM, 7}, {"EOR", &CPU::EOR, &CPU::DIR, 3}, {"LSR", &CPU::LSR, &CPU::DIR, 5}, {"EOR", &CPU::EOR, &CPU::DIL, 6}, {"PHA", &CPU::PHA, &CPU::STK, 3}, {"EOR", &CPU::EOR, &CPU::IMM, 2}, {"LSR", &CPU::LSR, &CPU::ACC, 2}, {"PHK", &CPU::PHK, &CPU::STK, 3}, {"JMP", &CPU::JMP, &CPU::ABS, 3}, {"EOR", &CPU::EOR, &CPU::ABS, 4}, {"LSR", &CPU::LSR, &CPU::ABS, 6}, {"EOR", &CPU::EOR, &CPU::ABL, 5},
        {"BVC", &CPU::BVC, &CPU::REL, 2}, {"EOR", &CPU::EOR, &CPU::DIX, 5}, {"EOR", &CPU::EOR, &CPU::DIN, 5}, {"EOR", &CPU::EOR, &CPU::SRII, 7}, {"MVN", &CPU::MVN, &CPU::BLM, 7}, {"EOR", &CPU::EOR, &CPU::DIRX, 4}, {"LSR", &CPU::LSR, &CPU::DIRX, 6}, {"EOR", &CPU::EOR, &CPU::DIXL, 6}, {"CLI", &CPU::CLI, &CPU::IMP, 2}, {"EOR", &CPU::EOR, &CPU::ABY, 4}, {"PHY", &CPU::PHY, &CPU::STK, 3}, {"TCD", &CPU::TCD, &CPU::IMP, 2}, {"JMP", &CPU::JMP, &CPU::ABL, 4}, {"EOR", &CPU::EOR, &CPU::ABX, 4}, {"LSR", &CPU::LSR, &CPU::ABX, 7}, {"EOR", &CPU::EOR, &CPU::ABLX, 5},
        {"RTS", &CPU::RTS, &CPU::STK, 6}, {"ADC", &CPU::ADC, &CPU::DXI, 6}, {"PER", &CPU::PER, &CPU::STK, 6}, {"ADC", &CPU::ADC, &CPU::SREL, 4}, {"STZ", &CPU::STZ, &CPU::DIR, 3}, {"ADC", &CPU::ADC, &CPU::DIR, 3}, {"ROR", &CPU::ROR, &CPU::DIR, 5}, {"ADC", &CPU::ADC, &CPU::DIL, 6}, {"PLA", &CPU::PLA, &CPU::STK, 4}, {"ADC", &CPU::ADC, &CPU::IMM, 2}, {"ROR", &CPU::ROR, &CPU::ACC, 2}, {"RTL", &CPU::RTL, &CPU::STK, 6}, {"JMP", &CPU::JMP, &CPU::ABI, 5}, {"ADC", &CPU::ADC, &CPU::ABS, 4}, {"ROR", &CPU::ROR, &CPU::ABS, 6}, {"ADC", &CPU::ADC, &CPU::ABL, 5},
        {"BVS", &CPU::BVS, &CPU::REL, 2}, {"ADC", &CPU::ADC, &CPU::DIX, 5}, {"ADC", &CPU::ADC, &CPU::DIN, 5}, {"ADC", &CPU::ADC, &CPU::SRII, 7}, {"STZ", &CPU::STZ, &CPU::DIRX, 4}, {"ADC", &CPU::ADC, &CPU::DIRX, 4}, {"ROR", &CPU::ROR, &CPU::DIRX, 6}, {"ADC", &CPU::ADC, &CPU::DIXL, 6}, {"SEI", &CPU::SEI, &CPU::IMP, 2}, {"ADC", &CPU::ADC, &CPU::ABY, 4}, {"PLY", &CPU::PLY, &CPU::STK, 4}, {"TDC", &CPU::TDC, &CPU::IMP, 2}, {"JMP", &CPU::JMP, &CPU::AII, 6}, {"ADC", &CPU::ADC, &CPU::ABX, 4}, {"ROR", &CPU::ROR, &CPU::ABX, 7}, {"ADC", &CPU::ADC, &CPU::ABLX, 5},
        {"BRA", &CPU::BRA, &CPU::REL, 2}, {"STA", &CPU::STA, &CPU::DXI, 6}, {"BRL", &CPU::BRL, &CPU::RELL, 4}, {"STA", &CPU::STA, &CPU::SREL, 4}, {"STY", &CPU::STY, &CPU::DIR, 3}, {"STA", &CPU::STA, &CPU::DIR, 3}, {"STX", &CPU::STX, &CPU::DIR, 3}, {"STA", &CPU::STA, &CPU::DIL, 2}, {"DEY", &CPU::DEY, &CPU::IMP, 2}, {"BIT", &CPU::BIT, &CPU::IMM, 2}, {"TXA", &CPU::TXA, &CPU::IMP, 2}, {"PHB", &CPU::PHB, &CPU::STK, 3}, {"STY", &CPU::STY, &CPU::ABS, 4}, {"STA", &CPU::STA, &CPU::ABS, 4}, {"STX", &CPU::STX, &CPU::ABS, 4}, {"STA", &CPU::STA, &CPU::ABL, 5},
        {"BCC", &CPU::BCC, &CPU::REL, 2}, {"STA", &CPU::STA, &CPU::DIX, 6}, {"STA", &CPU::STA, &CPU::DIN, 5}, {"STA", &CPU::STA, &CPU::SRII, 7}, {"STY", &CPU::STY, &CPU::DIRX, 4}, {"STA", &CPU::STA, &CPU::DIRX, 4}, {"STX", &CPU::STX, &CPU::DIRY, 4}, {"STA", &CPU::STA, &CPU::DIXL, 6}, {"TYA", &CPU::TYA, &CPU::IMP, 2}, {"STA", &CPU::STA, &CPU::ABY, 5}, {"TXS", &CPU::TXS, &CPU::IMP, 2}, {"TXY", &CPU::TXY, &CPU::IMP, 2}, {"STZ", &CPU::STZ, &CPU::ABS, 4}, {"STA", &CPU::STA, &CPU::ABX, 5}, {"STZ", &CPU::STZ, &CPU::ABX, 5}, {"STA", &CPU::STA, &CPU::ABLX, 5},
        {"LDY", &CPU::LDY, &CPU::IMM, 2}, {"LDA", &CPU::LDA, &CPU::DXI, 6}, {"LDX", &CPU::LDX, &CPU::IMM, 2}, {"LDA", &CPU::LDA, &CPU::SREL, 4}, {"LDY", &CPU::LDY, &CPU::DIR, 3}, {"LDA", &CPU::LDA, &CPU::DIR, 3}, {"LDX", &CPU::LDX, &CPU::DIR, 3}, {"LDA", &CPU::LDA, &CPU::DIL, 6}, {"TAY", &CPU::TAY, &CPU::IMP, 2}, {"LDA", &CPU::LDA, &CPU::IMM, 2}, {"TAX", &CPU::TAX, &CPU::IMP, 2}, {"PLB", &CPU::PLB, &CPU::STK, 4}, {"LDY", &CPU::LDY, &CPU::ABS, 4}, {"LDA", &CPU::LDA, &CPU::ABS, 4}, {"LDX", &CPU::LDX, &CPU::ABS, 4}, {"LDA", &CPU::LDA, &CPU::ABL, 5},
        {"BCS", &CPU::BCS, &CPU::REL, 2}, {"LDA", &CPU::LDA, &CPU::DIX, 5}, {"LDA", &CPU::LDA, &CPU::DIN, 5}, {"LDA", &CPU::LDA, &CPU::SRII, 7}, {"LDY", &CPU::LDY, &CPU::DIRX, 4}, {"LDA", &CPU::LDA, &CPU::DIRX, 4}, {"LDX", &CPU::LDX, &CPU::DIRY, 4}, {"LDA", &CPU::LDA, &CPU::DIXL, 6}, {"CLV", &CPU::CLV, &CPU::IMP, 2}, {"LDA", &CPU::LDA, &CPU::ABY, 4}, {"TSX", &CPU::TSX, &CPU::IMP, 2}, {"TYX", &CPU::TYX, &CPU::IMP, 2}, {"LDY", &CPU::LDY, &CPU::ABX, 4}, {"LDA", &CPU::LDA, &CPU::ABX, 4}, {"LDX", &CPU::LDX, &CPU::ABY, 4}, {"LDA", &CPU::LDA, &CPU::ABLX, 5},
        {"CPY", &CPU::CPY, &CPU::IMM, 2}, {"CMP", &CPU::CMP, &CPU::DXI, 6}, {"REP", &CPU::REP, &CPU::IMM, 3}, {"CMP", &CPU::CMP, &CPU::SREL, 4}, {"CPY", &CPU::CPY, &CPU::DIR, 3}, {"CMP", &CPU::CMP, &CPU::DIR, 3}, {"DEC", &CPU::DEC, &CPU::DIR, 5}, {"CMP", &CPU::CMP, &CPU::DIL, 6}, {"INY", &CPU::INY, &CPU::IMP, 2}, {"CMP", &CPU::CMP, &CPU::IMM, 2}, {"DEX", &CPU::DEX, &CPU::IMP, 2}, {"WAI", &CPU::WAI, &CPU::IMP, 3}, {"CPY", &CPU::CPY, &CPU::ABS, 4}, {"CMP", &CPU::CMP, &CPU::ABS, 4}, {"DEC", &CPU::DEC, &CPU::ABS, 6}, {"CMP", &CPU::CMP, &CPU::ABL, 5},
        {"BNE", &CPU::BNE, &CPU::REL, 2}, {"CMP", &CPU::CMP, &CPU::DIX, 5}, {"CMP", &CPU::CMP, &CPU::DIN, 5}, {"CMP", &CPU::CMP, &CPU::SRII, 7}, {"PEI", &CPU::PEI, &CPU::STK, 6}, {"CMP", &CPU::CMP, &CPU::DIRX, 4}, {"DEC", &CPU::DEC, &CPU::DIRX, 6}, {"CMP", &CPU::CMP, &CPU::DIXL, 6}, {"CLD", &CPU::CLD, &CPU::IMP, 2}, {"CMP", &CPU::CMP, &CPU::ABY, 4}, {"PHX", &CPU::PHX, &CPU::STK, 3}, {"STP", &CPU::STP, &CPU::IMP, 3}, {"JML", &CPU::JML, &CPU::ABI, 6}, {"CMP", &CPU::CMP, &CPU::ABX, 4}, {"DEC", &CPU::DEC, &CPU::ABX, 7}, {"CMP", &CPU::CMP, &CPU::ABLX, 5},
        {"CPX", &CPU::CPX, &CPU::IMM, 2}, {"SBC", &CPU::SBC, &CPU::DXI, 6}, {"SEP", &CPU::SEP, &CPU::IMM, 3}, {"SBC", &CPU::SBC, &CPU::SREL, 4}, {"CPX", &CPU::CPX, &CPU::DIR, 3}, {"SBC", &CPU::SBC, &CPU::DIR, 3}, {"INC", &CPU::INC, &CPU::DIR, 5}, {"SBC", &CPU::SBC, &CPU::DIL, 6}, {"INX", &CPU::INX, &CPU::IMP, 2}, {"SBC", &CPU::SBC, &CPU::IMM, 2}, {"NOP", &CPU::NOP, &CPU::IMP, 2}, {"XBA", &CPU::XBA, &CPU::IMP, 3}, {"CPX", &CPU::CPX, &CPU::ABS, 4}, {"SBC", &CPU::SBC, &CPU::ABS, 4}, {"INC", &CPU::INC, &CPU::ABS, 6}, {"SBC", &CPU::SBC, &CPU::ABL, 5},
        {"BEQ", &CPU::BEQ, &CPU::REL, 2}, {"SBC", &CPU::SBC, &CPU::DIX, 5}, {"SBC", &CPU::SBC, &CPU::DIN, 5}, {"SBC", &CPU::SBC, &CPU::SRII, 7}, {"PEA", &CPU::PEA, &CPU::STK, 5}, {"SBC", &CPU::SBC, &CPU::DIRX, 4}, {"INC", &CPU::INC, &CPU::DIRX, 6}, {"SBC", &CPU::SBC, &CPU::DIXL, 6}, {"SED", &CPU::SED, &CPU::IMP, 2}, {"SBC", &CPU::SBC, &CPU::ABY, 4}, {"PLX", &CPU::PLX, &CPU::STK, 4}, {"XCE", &CPU::XCE, &CPU::IMP, 2}, {"JSR", &CPU::JSR, &CPU::AII, 8}, {"SBC", &CPU::SBC, &CPU::ABX, 4}, {"INC", &CPU::INC, &CPU::ABX, 7}, {"SBC", &CPU::SBC, &CPU::ABLX, 5}
    };
}

CPU::~CPU()
{
}

void CPU::clearMemory() 
{
    for (auto& row : memory) {
        std::fill(row.begin(), row.end(), 0);
    }
}

void CPU::clearRegisters() {
    A = 0x0;
    X = 0x0;
    Y = 0x0;
    SP = 0x0;
    DBR = 0x0;
    DP = 0x0;
    PBR = 0x0;
    PC = 0x0;
    status = 0x0;
}

void CPU::clock()
{
    if (cycles == 0) {
        opcode = read((PBR << 16) | PC);
        PC++;

        cycles = lookup[opcode].cycles;

        uint8_t additional_cycles1 = (this->*lookup[opcode].address_mode)();
        uint8_t additional_cycles2 = (this->*lookup[opcode].operate)();

        cycles += (additional_cycles1 + additional_cycles2);
    }

    cycles--;
}

uint16_t CPU::fetch()
{
    if (!(lookup[opcode].address_mode == &CPU::IMP))
        fetched = (read(address_absolute+1) << 8) | read(address_absolute);
    return fetched;
}

uint8_t CPU::read(uint32_t address)
{
    printf("Reading from: bank: %02X, address: %04X\n", (address & 0xFF0000) >> 16, address & 0xFFFF);
    return memory[(address & 0xFF0000) >> 16][address & 0xFFFF];
}

void CPU::write(uint32_t address, uint8_t data)
{
    printf("Writing to: bank: %02X, address: %04X, val: %X\n", (address & 0xFF0000) >> 16, address & 0xFFFF, data);
    memory[(address & 0xFF0000) >> 16][address & 0xFFFF] = data;
}

void CPU::SetFlag(flags f, bool v)
{
    if (v) {
        status |= f;
    }
    else {
        status &= ~f;
    }
}

uint8_t CPU::GetFlag(flags f)
{
    return ((status & f) > 0) ? 1 : 0;
}

// addressing modes
uint8_t CPU::ABS()
{    
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint32_t tmp = (hi << 8) | lo;

    if (lookup[opcode].operate == &CPU::JMP || lookup[opcode].operate == &CPU::JSR) {
        tmp |= (PBR << 16);
    }
    else {
        tmp |= (DBR << 16);
    }
    
    address_absolute = tmp;

    return 0;
}

uint8_t CPU::ABX()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint32_t tmp = (DBR << 16) | (hi << 8) | lo;
    tmp += X;
    address_absolute = tmp;

    if ((hi << 8) != (tmp & 0xFF00)) {
        return 1;
    }    

    return 0;
}

uint8_t CPU::ABY()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint32_t tmp = (DBR << 16) | (hi << 8) | lo;
    tmp += Y;
    address_absolute = tmp;

    if ((hi << 8) != (tmp & 0xFF00)) {
        return 1;
    }

    return 0;
}

uint8_t CPU::AII()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint32_t tmp = (((hi << 8) | lo) + X) & 0xFFFF;
    tmp |= (PBR << 16); // wirklich PBR?

    lo = read(tmp);
    hi = read(tmp + 1);

    address_absolute = (PBR << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABI()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint32_t tmp = (hi << 8) | lo;
    lo = read(tmp);
    hi = read(tmp + 1);

    address_absolute = (PBR << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABL()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;
    uint16_t bank = read((PBR << 16) | PC);
    PC++;

    address_absolute = (bank << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABLX()
{
    uint16_t lo = read((PBR << 16) | PC);
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;
    uint16_t bank = read((PBR << 16) | PC);
    PC++;

    address_absolute = (((bank << 16) | (hi << 8) | lo) + X) & 0xFFFFFF;

    return 0;
}

uint8_t CPU::ACC()
{
    address_absolute = A; //PC verschieben?
    return 0;
}

uint8_t CPU::BLM()
{
    uint8_t lo = read((PBR << 16) | PC); 
    PC++;
    uint8_t hi = read((PBR << 16) | PC);
    PC++;
    address_absolute = ((DBR << 16) | (hi << 8) | lo) & 0xFFFFFF;
    return 0;
}

uint8_t CPU::DIR()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP;
    address_absolute = operand;
    return 0;
}

uint8_t CPU::DIN()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);

    address_absolute = (DBR << 16) | (hi << 8) | lo;
    return 0;
}

uint8_t CPU::DIX()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);

    uint32_t tmp = (DBR << 16) | (hi << 8) | lo;
    tmp += Y;
    address_absolute = tmp & 0xFFFFFF;

    return 0;
}

uint8_t CPU::DIXL()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);
    uint16_t bank = read(operand + 2);

    uint32_t tmp = (bank << 16) | (hi << 8) | lo;
    tmp += Y;
    address_absolute = tmp & 0xFFFFFF;

    return 0;
}

uint8_t CPU::DIL()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);
    uint16_t bank = read(operand + 2);

    address_absolute = (bank << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::DXI()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP + X;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);

    address_absolute = (DBR << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::DIRX()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP + X;

    address_absolute = operand & 0xFFFFFF;

    return 0;
}

uint8_t CPU::DIRY()
{
    uint16_t operand = read((PBR << 16) | PC);
    PC++;
    operand += DP + Y;

    address_absolute = operand & 0xFFFFFF;

    return 0;
}

uint8_t CPU::IMM() // TODO: Unterschied in Emulation Mode vs Native Mode!!!
{
    address_absolute = PC;
    PC += 2;
    return 0;
}

uint8_t CPU::IMP()
{
    PC++;
    return 0;
}

uint8_t CPU::REL()
{
    int16_t operand = (int8_t)read((PBR << 16) | PC);
    PC++;

    operand += PC;
    address_absolute = (PBR << 16) | operand;
    return 0;
}

uint8_t CPU::RELL()
{
    uint16_t lo = read((PBR << 16) | PC);    
    PC++;
    uint16_t hi = read((PBR << 16) | PC);
    PC++;

    uint16_t tmp = (int16_t)((hi << 8) | lo) + PC;
    address_absolute = (PBR << 16) | tmp;

    return 0;
}

uint8_t CPU::STK()
{
    PC++;
    address_absolute = SP; //Hier aufpassen, eigentlich sollte das nix machen, da die einzelnen instr. Die Arbeit machen, aber sonst wäre der address_abs undefined.
    return 0;
}

uint8_t CPU::SREL()
{
    int8_t operand = (int8_t)read((PBR << 16) | PC);
    address_absolute = SP + operand;

    return 0;
}

uint8_t CPU::SRII()
{
    uint8_t operand = read((PBR << 16) | PC);
    uint8_t lo = read(operand);
    uint8_t hi = read(operand + 1);
    uint16_t inaddr = (hi << 8) | lo;
    if (emulation_mode)
    {
        address_absolute = (Y & 0xFF) + inaddr;
    }
    else
    {
        address_absolute = Y + inaddr;
    }
    return 0;
}

// opcodes
uint8_t CPU::ADC()
{
    return uint8_t();
}

uint8_t CPU::AND()
{
    fetch();
    A &= fetched;

    SetFlag(Z, A == 0x0);
    SetFlag(N, A & 0x8000);

    return uint8_t();
}

uint8_t CPU::ASL()
{
    fetch();
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1; 
    uint32_t res = (uint32_t)fetched << 1;
    if (emulation_mode)
    {
        if (address_absolute & 0xFF000000)
        {
            zusatzCycle -= 1;
        }
        SetFlag(C, (res & 0xFFFFFF00));
        SetFlag(Z, ((res & 0xFF) == 0));
        SetFlag(N, (res & 0x80));
        if (lookup[opcode].operate == &CPU::IMP)
        {
            A = res & 0xFF;
        }
        else
        {
            write(address_absolute, (res & 0xFF));
        }
    }
    else
    {
        if (GetFlag(M))
        {
            zusatzCycle += 2;

        }
        
        SetFlag(C, (res & 0xFFFF0000));
        SetFlag(Z, ((res & 0xFFFF) == 0));
        SetFlag(N, (res & 0x8000));
        if (lookup[opcode].address_mode == &CPU::IMP)
        {
            A = res & 0xFFFF;
        }
        else
        {
            write(address_absolute, (res & 0xFFFF));
        }
    }

    return zusatzCycle;
}

uint8_t CPU::BCC()
{
    uint8_t additionalCycles = 0;
    if (GetFlag(C) == 0) {
        additionalCycles++;
        uint16_t previousPage = PC & 0xFF00;
        PC = address_absolute;

        if (emulation_mode) {
            if ((PC & 0xFF00) != previousPage) {
                additionalCycles++;
            }
        }
    }

    return additionalCycles;
}

uint8_t CPU::BCS()
{
    uint8_t additionalCycles = 0;
    if (GetFlag(C) == 1) {
        additionalCycles++;
        uint16_t previousPage = PC & 0xFF00;
        PC = address_absolute;

        if (emulation_mode) {
            if ((PC & 0xFF00) != previousPage) {
                additionalCycles++;
            }
        }
    }

    return additionalCycles;
}

uint8_t CPU::BEQ()
{
    uint8_t zusatzCycle = 0;
    if (GetFlag(Z))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BIT()
{
    fetch();

    if (emulation_mode) {
        SetFlag(N, fetched & 0x80);
        SetFlag(V, fetched & 0x40);
        SetFlag(Z, ((fetched & 0xFF) & (A & 0xFF)) != 0x0);
    }
    else {
        SetFlag(N, fetched & 0x8000);
        SetFlag(V, fetched & 0x4000);
        SetFlag(Z, (fetched & A) != 0x0);
    }

    return 0;
}

uint8_t CPU::BMI()
{
    uint8_t zusatzCycle = 0;
    if (GetFlag(N))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BNE()
{
    uint8_t zusatzCycle = 0;
    if (!GetFlag(Z))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BPL()
{
    uint8_t zusatzCycle = 0;
    if (!GetFlag(N))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BRA()
{
    uint8_t zusatzCycle = 0;
    int16_t val = (int8_t)fetch();
    PC += val;
    if (emulation_mode)
    {
        if (address_absolute & 0xFFFF0000)
        {
            zusatzCycle += 1;
        }
    }
    else
    {
        if (address_absolute & 0xFF000000)
        {
            zusatzCycle += 1;
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BRK()
{
    if (emulation_mode)
    {
        PC += 2;
        write(SP, PC & 0xff);
        SP--;
        write(SP, PC & 0xFF00);
        SP--;
        SetFlag(INDEX, true);
        SetFlag(I, true);
        uint8_t lo = read(0xFFFE);
        uint8_t hi = read(0xFFFF);
        PC = (hi << 8) | lo;
        return 0;
    }
    else
    {
        write(SP, PBR);
        SP--;
        PC += 2;
        write(SP, PC & 0xff);
        SP--;
        write(SP, PC & 0xFF00);
        SP--;
        write(SP, status);
        SP--;
        SetFlag(I, true);

        PBR = 0;
        uint8_t lo = read(0xFFE6);
        uint8_t hi = read(0xFFE7);
        PC = (hi << 8) | lo;
        SetFlag(D, 0);
        return 1;
    }
}

uint8_t CPU::BRL()
{
    int16_t val = (int16_t)fetch();
    PC += val;
    return 0;
}

uint8_t CPU::BVC()
{
    uint8_t zusatzCycle = 0;
    if (!GetFlag(V))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::BVS()
{
    uint8_t zusatzCycle = 0;
    if (GetFlag(V))
    {
        zusatzCycle += 1;
        int16_t val = (int8_t)fetch();
        PC += val;
        if (emulation_mode)
        {
            if (address_absolute & 0xFFFF0000)
            {
                zusatzCycle += 1;
            }
        }
        else
        {
            if (address_absolute & 0xFF000000)
            {
                zusatzCycle += 1;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::CLC()
{
    SetFlag(C, 0);
    return 0;
}

uint8_t CPU::CLD()
{
    SetFlag(D, 0);
    return 0;
}

uint8_t CPU::CLI()
{
    SetFlag(I, 0);
    return 0;
}

uint8_t CPU::CLV()
{
    SetFlag(V, 0);
    return 0;
}

uint8_t CPU::CMP()
{
    uint8_t zusatzCycle = 0;
    if (GetFlag(M))
    {
        zusatzCycle += 1;
    }
    if ((DP & 0xFF) != 0)
    {
        zusatzCycle += 1;
    }
    if (emulation_mode)
    {
        fetched = fetch() & 0xff;
        uint16_t res = (A & 0xff) - fetched;
        SetFlag(C, ((A & 0xff) >=  (fetched & 0xFF)));
        SetFlag(Z, res == 0);
        SetFlag(N, (res & 0x80));
    }
    else
    {
        fetch();
        uint16_t res = A - fetched;
        SetFlag(C, (A >= fetched));
        SetFlag(Z, res == 0);
        SetFlag(N, (res & 0x8000));
    }
    return zusatzCycle;
}

uint8_t CPU::CPX()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    fetch();
    uint16_t res = fetched - X;
    if (emulation_mode)
    {
        SetFlag(C, (X & 0xFF) >= (fetched & 0xFF));
        SetFlag(Z, ((res & 0xFF) == 0));
        SetFlag(N, (res & 0x80));
    }
    else
    {
        if (X == 0)
        {
            zusatzCycle += 1;
        }
        SetFlag(C, X >= fetched);
        SetFlag(Z, res == 0);
        SetFlag(N, (res & 0x8000));
    }
    return zusatzCycle;
}

uint8_t CPU::CPY()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    fetch();
    uint16_t res = fetched - Y;
    if (emulation_mode)
    {
        SetFlag(C, (X & 0xFF) >= (fetched & 0xFF));
        SetFlag(Z, ((res & 0xFF) == 0));
        SetFlag(N, (res & 0x80));
    }
    else
    {
        if (X == 0)
        {
            zusatzCycle += 1;
        }
        SetFlag(C, X >= fetched);
        SetFlag(Z, res == 0);
        SetFlag(N, (res & 0x8000));
    }
    return zusatzCycle;
}

uint8_t CPU::COP()
{
    if (emulation_mode)
    {
        PC += 2;
        write(SP, PC & 0xff);
        SP--;
        write(SP, PC & 0xFF00);
        SP--;
        write(SP, status);
        SP--;
        SetFlag(I, true);
        uint8_t lo = read(0xFFF4);
        uint8_t hi = read(0xFFF5);
        PC = (hi << 8) | lo;
        SetFlag(D, 0);
        return 0;
    }
    else
    {
        write(SP, PBR);
        SP--;
        PC += 2;
        write(SP, PC & 0xff);
        SP--;
        write(SP, PC & 0xFF00);
        SP--;
        write(SP, status);
        SP--;
        SetFlag(I, true);

        PBR = 0;
        uint8_t lo = read(0xFFE4);
        uint8_t hi = read(0xFFE5);
        PC = (hi << 8) | lo;
        SetFlag(D, 0);
        return 1;
    }
}

uint8_t CPU::DEC()
{
    uint8_t zusatzCycle = (DP & 0xFF) ? 0 : 1;
    if (lookup[opcode].address_mode == &CPU::IMP)
    {
        A--;
        if (emulation_mode)
        {
            SetFlag(N, (A & 0x80));
            SetFlag(Z, (A & 0xFF) == 0);
            if (!(address_absolute & 0xFFFF0000))
            {
                zusatzCycle -= 1;
            }
        }
        else
        {
            SetFlag(N, (A & 0x8000));
            SetFlag(Z, A == 0);
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
        }
    }
    else
    {
        fetch();
        fetched--;
        write(address_absolute, fetched  & 0xFF00);
        address_absolute++;
        write(address_absolute, fetched & 0xff);
        
        if (emulation_mode)
        {
            SetFlag(N, (fetched & 0x80));
            SetFlag(Z, (fetched & 0xFF) == 0);
        }
        else
        {
            SetFlag(N, (fetched & 0x8000));
            SetFlag(Z, fetched == 0);
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
        }
    }
    return zusatzCycle;
}

uint8_t CPU::DEX()
{
    X--;
    if (emulation_mode)
    {
        SetFlag(N, (X & 0x80));
        SetFlag(Z, (X & 0xFF) == 0);
    }
    else
    {
        SetFlag(N, (X & 0x8000));
        SetFlag(Z, X == 0);
    }
    return 0;
}

uint8_t CPU::DEY()
{
    Y--;
    if (emulation_mode)
    {
        SetFlag(N, (Y & 0x80));
        SetFlag(Z, (Y & 0xFF) == 0);
    }
    else
    {
        SetFlag(N, (Y & 0x8000));
        SetFlag(Z, Y == 0);
    }
    return 0;
}

uint8_t CPU::EOR()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    fetch();
    A = A ^ fetched;
    if (emulation_mode)
    {
        if (address_absolute & 0xFFFF0000)
        {
            zusatzCycle += 1;
        }
    }
    else
    {
        if (address_absolute & 0xFF000000)
        {
            zusatzCycle += 1;
        }
        if (!GetFlag(M))
        {
            zusatzCycle += 1;
        }
    }
    return zusatzCycle;
}

uint8_t CPU::INC()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;

    if (lookup[opcode].address_mode == &CPU::IMP)
    {
        A++;
        if (emulation_mode)
        {
            SetFlag(N, (A & 0x80));
            SetFlag(Z, (A & 0xFF) == 0);
            if (!(address_absolute & 0xFFFF0000))
            {
                zusatzCycle -= 1;
            }

        }
        else
        {
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
            SetFlag(N, (A & 0x8000));
            SetFlag(Z, A == 0);
        }
    }
    else
    {
        fetched++;
        if (emulation_mode)
        {
            SetFlag(N, (fetched & 0x80));
            SetFlag(Z, (fetched & 0xFF) == 0);

        }
        else
        {
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
            SetFlag(N, (fetched & 0x8000));
            SetFlag(Z, fetched == 0);
        }
    }
    return zusatzCycle;
}

uint8_t CPU::INX()
{
    X++;
    if (emulation_mode)
    {
        SetFlag(N, (X & 0x80));
        SetFlag(Z, (X & 0xFF) == 0);

    }
    else
    {
        SetFlag(N, (X & 0x8000));
        SetFlag(Z, X == 0);
    }
    return 0;
}

uint8_t CPU::INY()
{
    Y++;
    if (emulation_mode)
    {
        SetFlag(N, (Y & 0x80));
        SetFlag(Z, (Y & 0xFF) == 0);

    }
    else
    {
        SetFlag(N, (Y & 0x8000));
        SetFlag(Z, Y == 0);
    }
    return 0;
}

uint8_t CPU::JMP()
{
    PC = address_absolute;
    return 0;
}

uint8_t CPU::JML()
{
    PC = address_absolute;
    return 0;
}

uint8_t CPU::JSR() //TODO: JSR und JSL kommt in der Documentation 2x vor
{
    write(SP, PBR);
    SP--;
    write(SP, ((PC & 0xFF000) >> 8));
    SP--;
    write(SP, PC & 0xFF);
    SP--;
   
    PC = address_absolute;
    return 0;
}

uint8_t CPU::JSL() //TODO: JSR und JSL kommt in der Documentation 2x vor
{
    return uint8_t();
}

uint8_t CPU::LDA()
{
    fetch();
    A = fetched;
    return 0;
}

uint8_t CPU::LDX()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    fetch();
    if (emulation_mode)
    {
        if ((PC + fetched) & 0xFFFF0000)
        {
            zusatzCycle++;
        }
        X = fetched & 0xFF;
        SetFlag(N, (X & 0x80));
        SetFlag(Z, ((X & 0xFF) == 0));
    }
    else
    {
        if ((PC + fetched) & 0xFF000000)
        {
            zusatzCycle++;
        }
        if (X == 0)
        {
            zusatzCycle++;
        }
        X = fetched;
        SetFlag(N, (X & 0x8000));
        SetFlag(Z, (X == 0));
    }
    return zusatzCycle;
}

uint8_t CPU::LDY()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    fetch();
    if (emulation_mode)
    {
        if ((PC + fetched) & 0xFFFF0000)
        {
            zusatzCycle++;
        }
        Y = fetched & 0xFF;
        SetFlag(N, (Y & 0x80));
        SetFlag(Z, ((Y & 0xFF) == 0));
    }
    else
    {
        if ((PC + fetched) & 0xFF000000)
        {
            zusatzCycle++;
        }
        if (X == 0)
        {
            zusatzCycle++;
        }
        Y = fetched;
        SetFlag(N, (Y & 0x8000));
        SetFlag(Z, (Y == 0));
    }
    return zusatzCycle;
}

uint8_t CPU::LSR()
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    SetFlag(N, 0);
    if (lookup[opcode].address_mode == &CPU::ACC)
    {
        A = A >> 1;
        SetFlag(C, A & 0x1);
        if (emulation_mode)
        {
            SetFlag(Z, (A & 0xFF) == 0);
            if (!(PC & 0xFFFF0000))
            {
                zusatzCycle--;
            }
        }
        else
        {
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
            SetFlag(Z, A == 0);
        }
    }
    else
    {
        fetch();
        fetched = fetched >> 1;
        write(address_absolute, fetched);
        SetFlag(C, fetched & 0x1);
        if (emulation_mode)
        {
            SetFlag(Z, (fetched & 0xFF) == 0);
            if (!(PC & 0xFFFF0000))
            {
                zusatzCycle--;
            }
        }
        else
        {
            if (GetFlag(M))
            {
                zusatzCycle += 2;
            }
            SetFlag(Z, fetched == 0);
        }
    }
    return 0;
}

uint8_t CPU::MVN()
{
    uint8_t zusatzCycle = 0;
    fetch();
    uint8_t destBank = (fetched & 0xFF00 >> 8);
    uint8_t srcBank = (fetched & 0xFF);
    if (emulation_mode || GetFlag(INDEX))
    {
        destBank = 0;
        srcBank = 0;
    }
    while (A >= 0)
    {
        write((destBank << 16) | Y, read((srcBank << 16) | X));
        X++;
        Y++;
        A--;
        zusatzCycle += 7;
    }
    return zusatzCycle;
}

uint8_t CPU::MVP()
{
    uint8_t zusatzCycle = 0;
    fetch();
    uint8_t destBank = (fetched & 0xFF00 >> 8);
    uint8_t srcBank = (fetched & 0xFF);
    if (emulation_mode || GetFlag(INDEX))
    {
        destBank = 0;
        srcBank = 0;
    }
    while (A >= 0)
    {
        write((destBank << 16) | Y, read((srcBank << 16) | X));
        X--;
        Y--;
        A--;
        zusatzCycle += 7;
    }
    return zusatzCycle;
}

uint8_t CPU::NOP()
{
    return 0;
}

uint8_t CPU::ORA() // Hier ist mir aufgefallen, dass die Accumulator/X/Y register unabhänging vom emulationMode in 8-bit modus gebracht werden können...
{
    uint8_t zusatzCycle = ((DP & 0xFF) == 0) ? 0 : 1;
    if (GetFlag(M))
    {
        zusatzCycle++;
    }

    fetch();
    A = A | fetched;
    if (emulation_mode)
    {
        SetFlag(N, (A & 0x80));
        SetFlag(Z, (A & 0xFF) == 0);
        if (((PC + X)) & 0xFFFF0000)
        {
            zusatzCycle++;
        }
    }
    else
    {
        SetFlag(N, (A & 0x8000));
        SetFlag(Z, A  == 0);
        if (((PC + X)) & 0xFF000000)
        {
            zusatzCycle++;
        }
    }
    return zusatzCycle;
}

uint8_t CPU::PEA()
{
    uint8_t lo = read((PBR << 16) | PC);// wie in STK angemerkt zieht sich die Inst. die daten selber
    PC++;
    uint8_t hi = read((PBR << 16) | PC);
    PC++;
    write(SP, hi);
    SP--;
    write(SP, lo);
    SP--;
    return 0;
}

uint8_t CPU::PEI()
{
    uint8_t lo = read((PBR << 16) | PC);// wie in STK angemerkt zieht sich die Inst. die daten selber
    PC++;

    uint8_t zusatzCycle = (DP == 0) ? 0 : 1;
    uint16_t res = DP + lo;

    uint8_t high = read((DBR << 16) | (res + 1));
    uint8_t low = read((DBR << 16) | (res));

    write(SP, high);
    SP--;
    write(SP, low);
    SP--;

    return zusatzCycle;
}

uint8_t CPU::PER()
{
    int16_t signedFetch = (int16_t)fetch();
    uint16_t res = PC + signedFetch;
    write(SP, ((res & 0xFF00) >> 8));
    SP--;
    write(SP, ((res & 0xFF)));
    SP--;
    return 0;
}

uint8_t CPU::PHA()
{
    if (emulation_mode | GetFlag(M))
    {
        write(SP, (A & 0xFF));
        SP--;
    }
    else
    {
        write(SP, ((A & 0xFF00) >> 8));
        SP--;
        write(SP, (A & 0xFF));
        SP--;
    }
    if (GetFlag(M))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t CPU::PHB()
{
    write(SP, DBR);
    SP--;
    return 0;
}

uint8_t CPU::PHD()
{
    write(SP, ((DP & 0xFF00) >> 8));
    SP--;
    write(SP, (DP & 0xFF));
    SP--;
    return 0;
}

uint8_t CPU::PHK()
{
    write(SP, PBR);
    SP--;
    return 0;
}

uint8_t CPU::PHP()
{
    write(SP, status);
    SP--;
    return 0;
}

uint8_t CPU::PHX()
{
    if (emulation_mode || GetFlag(INDEX))
    {
        write(SP, (X & 0xFF));
        SP--;
    }
    else
    {
        write(SP, ((X & 0xFF00) >> 8));
        SP--;
        write(SP, (X & 0xFF));
        SP--;
    }
    if (GetFlag(INDEX))
    {
        return 0;
    }
    else
    {
        return 1;
    } 
}

uint8_t CPU::PHY()
{
    if (emulation_mode || GetFlag(INDEX))
    {
        write(SP, (Y & 0xFF));
        SP--;
    }
    else
    {
        write(SP, ((Y & 0xFF00) >> 8));
        SP--;
        write(SP, (Y & 0xFF));
        SP--;
    }
    if (GetFlag(INDEX))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t CPU::PLA()
{
    SP++;
    if (emulation_mode || GetFlag(M))
    {
        A = read(SP);
        SetFlag(N, (A & 0x80));
        SetFlag(Z, ((A & 0xFF) == 0));
    }
    else
    {
        uint8_t lo = read(SP);
        SP++;
        uint8_t hi = read(SP);
        A = (hi << 8) | lo;
        SetFlag(N, (A & 0x8000));
        SetFlag(Z, (A == 0));
    }
    if(GetFlag(M))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t CPU::PLB()
{
    SP++;
    DBR = read(SP);
    SetFlag(N, (DBR & 0x80));
    SetFlag(Z, (DBR == 0));
    return 0;
}

uint8_t CPU::PLD()
{
    SP++;
    uint8_t lo = read(SP);
    SP++;
    uint8_t hi = read(SP);
    DP = (hi << 8) | lo;
    SetFlag(N, (DP & 0x8000));
    SetFlag(Z, (DP == 0));
    return 0;
}

uint8_t CPU::PLP()
{
    SP++;
    status = read(SP);
    return uint8_t();
}

uint8_t CPU::PLX()
{
    SP++;
    if (emulation_mode || GetFlag(INDEX))
    {
        X = read(SP);
        SetFlag(N, (X & 0x80));
        SetFlag(Z, ((X & 0xFF) == 0));
    }
    else
    {
        uint8_t lo = read(SP);
        SP++;
        uint8_t hi = read(SP);
        X = (hi << 8) | lo;
        SetFlag(N, (X & 0x8000));
        SetFlag(Z, (X == 0));
    }

    if (GetFlag(INDEX))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t CPU::PLY()
{
    SP++;
    if (emulation_mode || GetFlag(INDEX))
    {
        Y = read(SP);
        SetFlag(N, (Y & 0x80));
        SetFlag(Z, ((Y & 0xFF) == 0));
    }
    else
    {
        uint8_t lo = read(SP);
        SP++;
        uint8_t hi = read(SP);
        Y = (hi << 8) | lo;
        SetFlag(N, (Y & 0x8000));
        SetFlag(Z, (Y == 0));
    }

    if (GetFlag(INDEX))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint8_t CPU::REP()
{
    fetch();
    status = ~(status & (fetched & 0xFF)) | status;
    return 0;
}

uint8_t CPU::ROL()
{
    return uint8_t();
}

uint8_t CPU::ROR()
{
    return uint8_t();
}

uint8_t CPU::RTI()
{
    return uint8_t();
}

uint8_t CPU::RTS()
{
    return uint8_t();
}

uint8_t CPU::RTL()
{
    return uint8_t();
}

uint8_t CPU::SBC()
{
    return uint8_t();
}

uint8_t CPU::SEC()
{
    SetFlag(C, 1);
    return 0;
}

uint8_t CPU::SED()
{
    SetFlag(D, 1);
    return 0;
}

uint8_t CPU::SEI()
{
    SetFlag(I, 1);
    return 0;
}

uint8_t CPU::SEP()
{
    return uint8_t();
}

uint8_t CPU::STA()
{
    return uint8_t();
}

uint8_t CPU::STX()
{
    return uint8_t();
}

uint8_t CPU::STY()
{
    return uint8_t();
}

uint8_t CPU::STP()
{
    return uint8_t();
}

uint8_t CPU::STZ()
{
    return uint8_t();
}

uint8_t CPU::TAX()
{
    return uint8_t();
}

uint8_t CPU::TAY()
{
    return uint8_t();
}

uint8_t CPU::TCD()
{
    return uint8_t();
}

uint8_t CPU::TCS()
{
    return uint8_t();
}

uint8_t CPU::TDC()
{
    return uint8_t();
}

uint8_t CPU::TSC()
{
    return uint8_t();
}

uint8_t CPU::TSX()
{
    return uint8_t();
}

uint8_t CPU::TXA()
{
    return uint8_t();
}

uint8_t CPU::TXS()
{
    return uint8_t();
}

uint8_t CPU::TXY()
{
    return uint8_t();
}

uint8_t CPU::TYA()
{
    return uint8_t();
}

uint8_t CPU::TYX()
{
    return uint8_t();
}

uint8_t CPU::TRB()
{
    return uint8_t();
}

uint8_t CPU::TSB()
{
    return uint8_t();
}

uint8_t CPU::WAI()
{
    return uint8_t();
}

uint8_t CPU::WDM()
{
    return uint8_t();
}

uint8_t CPU::XBA()
{
    return uint8_t();
}

uint8_t CPU::XCE()
{
    bool prev = emulation_mode;
    emulation_mode = GetFlag(C);
    SetFlag(C, prev);
    if (!emulation_mode)
    {
         SetFlag(M, true);
         SetFlag(INDEX, true);
    }
    return 0;
}