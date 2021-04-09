#include "CPU.h"

CPU::CPU()
{
    memory.resize(256);
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
        opcode = read(PC);
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
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
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
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
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
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
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
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
    PC++;

    uint32_t tmp = (((hi << 8) | lo) + X) & 0xFFFF;
    tmp |= (PBR << 16);

    lo = read(tmp);
    hi = read(tmp + 1);

    address_absolute = (PBR << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABI()
{
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
    PC++;

    uint32_t tmp = (hi << 8) | lo;
    lo = read(tmp);
    hi = read(tmp + 1);

    address_absolute = (PBR << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABL()
{
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
    PC++;
    uint16_t bank = read(PC);
    PC++;

    address_absolute = (bank << 16) | (hi << 8) | lo;

    return 0;
}

uint8_t CPU::ABLX()
{
    uint16_t lo = read(PC);
    PC++;
    uint16_t hi = read(PC);
    PC++;
    uint16_t bank = read(PC);
    PC++;

    address_absolute = (((bank << 16) | (hi << 8) | lo) + X) & 0xFFFFFF;

    return 0;
}

uint8_t CPU::ACC()
{
    address_absolute = A;
    return 0;
}

uint8_t CPU::BLM()
{
    return uint8_t();
}

uint8_t CPU::DIR()
{
    uint16_t operand = read(PC);
    PC++;
    operand += DP;
    address_absolute = operand;
    return 0;
}

uint8_t CPU::DIN()
{
    uint16_t operand = read(PC);
    PC++;
    operand += DP;

    uint16_t lo = read(operand);
    uint16_t hi = read(operand + 1);

    address_absolute = (DBR << 16) | (hi << 8) | lo;
    return 0;
}

uint8_t CPU::DIX()
{
    uint16_t operand = read(PC);
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
    uint16_t operand = read(PC);
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
    return uint8_t();
}

uint8_t CPU::DXI()
{
    return uint8_t();
}

uint8_t CPU::DIRX()
{
    return uint8_t();
}

uint8_t CPU::DIRY()
{
    return uint8_t();
}

uint8_t CPU::IMM()
{
    address_absolute = PC;
    PC += 2;
    return 0;
}

uint8_t CPU::IMP()
{
    return 0;
}

uint8_t CPU::REL()
{
    return uint8_t();
}

uint8_t CPU::RELL()
{
    return uint8_t();
}

uint8_t CPU::STK()
{
    return uint8_t();
}

uint8_t CPU::SREL()
{
    return uint8_t();
}

uint8_t CPU::SRII()
{
    return uint8_t();
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
    return uint8_t();
}

uint8_t CPU::BCC()
{
    return uint8_t();
}

uint8_t CPU::BCS()
{
    return uint8_t();
}

uint8_t CPU::BEQ()
{
    return uint8_t();
}

uint8_t CPU::BIT()
{
    return uint8_t();
}

uint8_t CPU::BMI()
{
    return uint8_t();
}

uint8_t CPU::BNE()
{
    return uint8_t();
}

uint8_t CPU::BPL()
{
    return uint8_t();
}

uint8_t CPU::BRA()
{
    return uint8_t();
}

uint8_t CPU::BRK()
{
    return uint8_t();
}

uint8_t CPU::BRL()
{
    return uint8_t();
}

uint8_t CPU::BVC()
{
    return uint8_t();
}

uint8_t CPU::BVS()
{
    return uint8_t();
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
    return uint8_t();
}

uint8_t CPU::CPX()
{
    return uint8_t();
}

uint8_t CPU::CPY()
{
    return uint8_t();
}

uint8_t CPU::COP()
{
    return uint8_t();
}

uint8_t CPU::DEC()
{
    return uint8_t();
}

uint8_t CPU::DEX()
{
    return uint8_t();
}

uint8_t CPU::DEY()
{
    return uint8_t();
}

uint8_t CPU::EOR()
{
    return uint8_t();
}

uint8_t CPU::INC()
{
    return uint8_t();
}

uint8_t CPU::INX()
{
    return uint8_t();
}

uint8_t CPU::INY()
{
    return uint8_t();
}

uint8_t CPU::JMP()
{
    PC = address_absolute;
    return 0;
}

uint8_t CPU::JML()
{
    return uint8_t();
}

uint8_t CPU::JSR()
{
    return uint8_t();
}

uint8_t CPU::JSL()
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
    return uint8_t();
}

uint8_t CPU::LDY()
{
    return uint8_t();
}

uint8_t CPU::LSR()
{
    return uint8_t();
}

uint8_t CPU::MVN()
{
    return uint8_t();
}

uint8_t CPU::MVP()
{
    return uint8_t();
}

uint8_t CPU::NOP()
{
    return uint8_t();
}

uint8_t CPU::ORA()
{
    return uint8_t();
}

uint8_t CPU::PEA()
{
    return uint8_t();
}

uint8_t CPU::PEI()
{
    return uint8_t();
}

uint8_t CPU::PER()
{
    return uint8_t();
}

uint8_t CPU::PHA()
{
    return uint8_t();
}

uint8_t CPU::PHB()
{
    return uint8_t();
}

uint8_t CPU::PHD()
{
    return uint8_t();
}

uint8_t CPU::PHK()
{
    return uint8_t();
}

uint8_t CPU::PHP()
{
    return uint8_t();
}

uint8_t CPU::PHX()
{
    return uint8_t();
}

uint8_t CPU::PHY()
{
    return uint8_t();
}

uint8_t CPU::PLA()
{
    return uint8_t();
}

uint8_t CPU::PLB()
{
    return uint8_t();
}

uint8_t CPU::PLD()
{
    return uint8_t();
}

uint8_t CPU::PLP()
{
    return uint8_t();
}

uint8_t CPU::PLX()
{
    return uint8_t();
}

uint8_t CPU::PLY()
{
    return uint8_t();
}

uint8_t CPU::REP()
{
    return uint8_t();
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
    return uint8_t();
}