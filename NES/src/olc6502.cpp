#include "olc6502.h"
#include "Bus.h"
#include <cstdint>

olc6502::olc6502() {
  using a = olc6502;
  lookup = {
      { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
    { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
    { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
    { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
    { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
  };
}

olc6502::~olc6502() {}

uint8_t olc6502::cpuRead(uint16_t address) { return bus->cpuRead(address, false); }

void olc6502::cpuWrite(uint16_t address, uint8_t data) {
  bus->cpuWrite(address, data);
}

void olc6502::clock() {
  if (cycles == 0) {
    opcode = cpuRead(pc);
    SetFlag(U, true);
    pc++;
    cycles = lookup[opcode].cycles;

    uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();
    uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

    cycles += (additional_cycle1 & additional_cycle2);
    SetFlag(U, true);
  }
  cycles--;
}

void olc6502::reset() {
  a = 0;
  x = 0;
  y = 0;
  stkp = 0xFD;
  status = 0x00 | U;

  addr_abs = 0xFFFC;
  uint16_t lo = cpuRead(addr_abs);
  uint16_t hi = cpuRead(addr_abs + 1);

  pc = (hi << 8) | lo;

  addr_rel = 0x0000;
  addr_abs = 0x0000;
  fetched = 0x00;

  cycles = 8;
}

void olc6502::irq() {
  if (GetFlag(I) == 0) {
    cpuWrite(0x0100 + stkp, (pc >> 8) & 0x00FF);
    stkp--;
    cpuWrite(0x0100 + stkp, pc & 0x00FF);
    stkp--;

    SetFlag(B, 0);
    SetFlag(U, 1);
    SetFlag(I, 1);
    cpuWrite(0x0100 + stkp, status);
    stkp--;

    uint16_t lo = cpuRead(addr_abs);
    uint16_t hi = cpuRead(addr_abs + 1);
    pc = (hi << 8) | lo;

    cycles = 7;
  }
}

void olc6502::nmi() {
  cpuWrite(0x0100 + stkp, (pc >> 8) & 0x00FF);
  stkp--;
  cpuWrite(0x0100 + stkp, pc & 0x00FF);
  stkp--;

  SetFlag(B, 0);
  SetFlag(U, 1);
  SetFlag(I, 1);
  cpuWrite(0x0100 + stkp, status);
  stkp--;

  uint16_t lo = cpuRead(addr_abs);
  uint16_t hi = cpuRead(addr_abs + 1);
  pc = (hi << 8) | lo;

  cycles = 7;
}

////////////////////////////////////////////////////////////
// Flag Functions
uint8_t olc6502::GetFlag(FLAGS6502 flag) {
		return ((status & flag) > 0 ? 1 : 0);
}

void olc6502::SetFlag(FLAGS6502 flag, bool v) {
  if (v)
    status |= flag;
  else
    status &= ~flag;
}

//////////////////////////////////////////////////////////
// Addressing Modes
uint8_t olc6502::IMP() {
  fetched = a;
  return 0;
}

uint8_t olc6502::IMM() {
  addr_abs = pc++;
  return 0;
}

uint8_t olc6502::ZP0() {
  addr_abs = cpuRead(pc);
  pc++;
  addr_abs &= 0x00FF;
  return 0;
}

uint8_t olc6502::ZPX() {
  addr_abs = (cpuRead(pc) + x);
  pc++;
  addr_abs &= 0x00FF;
  return 0;
}

uint8_t olc6502::ZPY() {
  addr_abs = (cpuRead(pc) + y);
  pc++;
  addr_abs &= 0x00FF;
  return 0;
}

uint8_t olc6502::ABS() {
  uint16_t lo = cpuRead(pc);
  pc++;
  uint16_t hi = cpuRead(pc);
  pc++;

  addr_abs = (hi << 8) | lo;

  return 0;
}

uint8_t olc6502::ABX() {
  uint16_t lo = cpuRead(pc);
  pc++;
  uint16_t hi = cpuRead(pc);
  pc++;

  addr_abs = (hi << 8) | lo;
  addr_abs += x;

  return (addr_abs & 0xFF00) != (hi << 8) ? 1 : 0;
}

uint8_t olc6502::ABY() {
  uint16_t lo = cpuRead(pc);
  pc++;
  uint16_t hi = cpuRead(pc);
  pc++;

  addr_abs = (hi << 8) | lo;
  addr_abs += y;

  return (addr_abs & 0xFF00) != (hi << 8) ? 1 : 0;
}

uint8_t olc6502::IND() {
  uint16_t ptr_lo = cpuRead(pc);
  pc++;
  uint16_t ptr_hi = cpuRead(pc);
  pc++;

  uint16_t ptr = (ptr_hi << 8) | ptr_lo;

  if (ptr_lo == 0x00FF)
    addr_abs = (cpuRead(ptr & 0x00FF) << 8) | cpuRead(ptr + 0);
  else
    addr_abs = (cpuRead(ptr + 1) << 8) | cpuRead(ptr + 0);

  return 0;
}

uint8_t olc6502::IZX() {
  uint16_t t = cpuRead(pc);
  pc++;

  uint16_t lo = cpuRead((uint16_t)(t + (uint16_t)x) & 0x00FF);
  uint16_t hi = cpuRead((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

  addr_abs = (hi << 8) | lo;

  return 0;
}

uint8_t olc6502::IZY() {
  uint16_t t = cpuRead(pc);
  pc++;

  uint16_t lo = cpuRead(t & 0x00FF);
  uint16_t hi = cpuRead((t + 1) & 0x00FF);

  addr_abs = (hi << 8) | lo;
  addr_abs += y;

  return (addr_abs & 0x00FF) != (hi << 8) ? 1 : 0;
}

uint8_t olc6502::REL() {
  addr_rel = cpuRead(pc);
  pc++;
  if (addr_rel & 0x80)
    addr_rel |= 0xFF00;
  return 0;
}

//////////////////////////////////////////////////////
// Instructions (opcodes)
uint8_t olc6502::fetch() {
  if (!(lookup[opcode].addrmode == &olc6502::IMP))
    fetched = cpuRead(addr_abs);
  return fetched;
}

// Add with carry
uint8_t olc6502::ADC() {
  fetch();
  uint16_t temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
  SetFlag(C, temp > 255);
  SetFlag(Z, (temp & 0x00FF) == 0);
  SetFlag(N, temp & 0x80);
  SetFlag(V, (((uint16_t)a ^ (uint16_t)temp) &
              (~((uint16_t)a ^ (uint16_t)fetched))) &
                 0x0080);
  a = temp & 0x00FF;
  return 1;
}

// Logical and
uint8_t olc6502::AND() {
  fetch();
  a &= fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

// Arithmetic shift left
uint8_t olc6502::ASL() {
  fetch();
  temp = (uint16_t)fetched << 1;
  SetFlag(C, (temp & 0xFF00) > 0);
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  SetFlag(N, (temp & 0x80));
  if (lookup[opcode].addrmode == &olc6502::IMP)
    a = temp & 0x00FF;
  else
    cpuWrite(addr_abs, temp & 0x00FF);
  return 0;
}

// Branch on carry clear
uint8_t olc6502::BCC() {
  if (GetFlag(C) == 0) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Branch on carry set
uint8_t olc6502::BCS() {
  if (GetFlag(C) == 1) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Branch on result zero
uint8_t olc6502::BEQ() {
  if (GetFlag(Z) == 1) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Test bits in memory with acumulator
uint8_t olc6502::BIT() {
  fetch();
  temp = a & fetched;
  SetFlag(N, temp & 0x80); // 0x80 == (1 << 7)
  SetFlag(V, temp & 0x40); // 0x40 == (1 << 6)
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  return 0;
}

// Branch on result minus
uint8_t olc6502::BMI() {
  if (GetFlag(N) == 1) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Branch on result not zero
uint8_t olc6502::BNE() {
	if (GetFlag(Z) == 0){
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Branch on result plus
uint8_t olc6502::BPL() {
  if (GetFlag(N) == 0) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

// Force Break
uint8_t olc6502::BRK() {
  pc++;

  SetFlag(I, 1);
  cpuWrite(0x0100 + stkp, (pc >> 8) & 0x00FF);
  stkp--;
  cpuWrite(0x0100 + stkp, pc & 0x00FF);
  stkp--;

  SetFlag(B, 1);
  cpuWrite(0x0100 + stkp, status);
  stkp--;
  SetFlag(B, 0);

  pc = (uint16_t)cpuRead(0xFFFE) | ((uint16_t)cpuRead(0xFFFF) << 8);
  return 0;
}

//Branch on overflow clear
uint8_t olc6502::BVC() {
  if (GetFlag(V) == 0) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

//Branch on overflow set
uint8_t olc6502::BVS() {
  if (GetFlag(V) == 1) {
    cycles++;
    addr_abs = pc + addr_rel;

    if ((addr_abs & 0xFF00) != (pc & 0xFF00))
      cycles++;

    pc = addr_abs;
  }
  return 0;
}

//Clear carry
uint8_t olc6502::CLC() {
  SetFlag(C, false);
  return 0;
}

//Clear decimal
uint8_t olc6502::CLD() {
  SetFlag(D, false);
  return 0;
}

//Clear interrupt disable
uint8_t olc6502::CLI() {
  SetFlag(I, false);
  return 0;
}

//Clear overflow
uint8_t olc6502::CLV() {
  SetFlag(V, false);
  return 0;
}

//Compare memory with accumulator
uint8_t olc6502::CMP()
{
  fetch();
  temp = (uint16_t)a - (uint16_t)fetched;
  SetFlag(C, a >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 1;
}

//Compare x register with accumulator
uint8_t olc6502::CPX()
{
  fetch();
  temp = (uint16_t)x - (uint16_t)fetched;
  SetFlag(C, x >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 1;
}

//Compare y register with accumulator
uint8_t olc6502::CPY()
{
  fetch();
  temp = (uint16_t)y - (uint16_t)fetched;
  SetFlag(C, y >= fetched);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 1;
}

//Decrease memory by one
uint8_t olc6502::DEC()
{
  fetch();
  temp = (uint16_t)fetched - 1;
  cpuWrite(addr_abs, temp & 0x00FF);
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 0;
}

//Decrease x register by one
uint8_t olc6502::DEX()
{
  x--;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

//Decrease y register by one
uint8_t olc6502::DEY()
{
  y--;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 0;
}

//Exclusive or memory and accumulator
uint8_t olc6502::EOR()
{
  fetch();
  a = fetched ^ a;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

//Increase memory by one
uint8_t olc6502::INC()
{
  fetch();
  temp = (uint16_t)fetched + 1;
  cpuWrite(addr_abs, (fetched & 0x00FF));
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  return 1;
}

//Increase x register by one
uint8_t olc6502::INX()
{
  x++;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

//Increase y register by one
uint8_t olc6502::INY()
{
  y++;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 0;
}

//Jump to new location
uint8_t olc6502::JMP()
{
  pc = addr_abs;
  return 0;
}

//Jump to new location saving return address
uint8_t olc6502::JSR()
{
  pc--;
  cpuWrite(0x0100 + stkp, (pc >> 8) & 0x00FF);
  stkp--;
  cpuWrite(0x0100 + stkp, pc & 0x00FF);
  stkp--;

  pc = addr_abs;
  return 0;
}

//Load index a
uint8_t olc6502::LDA()
{
  fetch();
  a = fetched;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

//Load index x
uint8_t olc6502::LDX()
{
  fetch();
  x = fetched;
  SetFlag(Z, x == 0x00);
  SetFlag(N, x & 0x80);
  return 0;
}

//Load index y
uint8_t olc6502::LDY()
{
  fetch();
  y = fetched;
  SetFlag(Z, y == 0x00);
  SetFlag(N, y & 0x80);
  return 0;
}

//Shift one bit right
uint8_t olc6502::LSR()
{
  fetch();
  SetFlag(C, fetched & 0x0001);
  temp = fetched >> 1;
  SetFlag(Z, (temp & 0x00FF) == 0x0000);
  SetFlag(N, temp & 0x0080);
  if (lookup[opcode].addrmode == &olc6502::IMP)
    a = temp & 0x00FF;
  else
    cpuWrite(addr_abs, temp & 0x00FF);
  return 0;
}

//No operation
uint8_t olc6502::NOP()
{
  switch (opcode) {
  case 0x1C:
  case 0x3C:
  case 0x5C:
  case 0x7C:
  case 0xDC:
  case 0xFC:
    return 1;
    break;
  }
  return 0;
}

//Or memory with accumulator
uint8_t olc6502::ORA()
{
  fetch();
  a = fetched | a;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 1;
}

//Push accumulator on stack
uint8_t olc6502::PHA()
{
  cpuWrite(0x0100 + stkp, a);
  stkp--;
  return 0;
}

//Push status on stack
uint8_t olc6502::PHP()
{
  cpuWrite(0x0100 + stkp, status | B | U);
  SetFlag(B, 0);
  SetFlag(U, 0);
  stkp--;
  return 0;
}

//Pull accumulator on the stack
uint8_t olc6502::PLA() {
  stkp++;
  a = cpuRead(0x100 + stkp);
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

//Pull status from the stack
uint8_t olc6502::PLP()
{
  stkp++;
  status = cpuRead(0x0100 + stkp);
  SetFlag(U, 1);
  return 0;
}

//Rotate one bit left
uint8_t olc6502::ROL()
{
  fetch();
  temp = (uint16_t)(fetched << 1) | GetFlag(C);
  SetFlag(C, temp & 0xFF00);
  SetFlag(Z, (temp & 0x00FF) & 0x0000);
  SetFlag(N, temp & 0x0080);
  if (lookup[opcode].addrmode == &olc6502::IMP)
    a = temp & 0x00FF;
  else
    cpuWrite(addr_abs, temp & 0x00FF);
  return 0;
}

//Rotate one bit right
uint8_t olc6502::ROR()
{
  fetch();
  temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
  SetFlag(C, fetched & 0x01);
  SetFlag(Z, (temp & 0x00FF) == 0x00);
  SetFlag(N, temp & 0x0080);
  if (lookup[opcode].addrmode == &olc6502::IMP)
    a = temp & 0x00FF;
  else
    cpuWrite(addr_abs, temp & 0x00FF);
  return 0;
}

//Return from interrupt
uint8_t olc6502::RTI() {
  stkp++;
  status = cpuRead(0x0100 + stkp);
  status &= ~B;
  status &= ~U;

  stkp++;
  pc = (uint16_t)cpuRead(0x0100 + stkp);
  stkp++;
  pc |= (uint16_t)cpuRead(0x100 + stkp) << 8;
  return 0;
}

//Return from subroutine
uint8_t olc6502::RTS() {
  stkp++;
  pc = (uint16_t)cpuRead(0x0100 + stkp);
  stkp++;
  pc |= (uint16_t)cpuRead(0x0100 + stkp) << 8;

  pc++;
  return 0;
}

//Substract with borrow
uint8_t olc6502::SBC() {
  fetch();
  uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

  uint16_t temp = (uint16_t)a + (uint16_t)value + (uint16_t)GetFlag(C);
  SetFlag(C, temp > 255);
  SetFlag(Z, (temp & 0x00FF) == 0);
  SetFlag(N, temp & 0x80);
  SetFlag(
      V, (((uint16_t)a ^ (uint16_t)temp) & (~((uint16_t)a ^ (uint16_t)value))) &
             0x0080);
  a = temp & 0x00FF;
  return 1;
}

//Set carry flag
uint8_t olc6502::SEC() {
  SetFlag(C, 1);
  return 0;
}

//Set decimal flag
uint8_t olc6502::SED() {
  SetFlag(D, 1);
  return 0;
}

//Set interrupt disable flag
uint8_t olc6502::SEI() {
  SetFlag(I, 1);
  return 0;
}

//Store accumulator in memory
uint8_t olc6502::STA() {
  cpuWrite(addr_abs, a);
  return 0;
}

//Store index x in memory
uint8_t olc6502::STX() {
  cpuWrite(addr_abs, x);
  return 0;
}

//Store index y in memory
uint8_t olc6502::STY() {
  cpuWrite(addr_abs, y);
  return 0;
}

//Transfer accumulator to x register
uint8_t olc6502::TAX() {
  x = a;
  SetFlag(N, x & 0x80);
  SetFlag(Z, x == 0x00);
  return 0;
}

//Transfer accumulator to y register
uint8_t olc6502::TAY() {
  y = a;
  SetFlag(N, y & 0x80);
  SetFlag(Z, y == 0x00);
  return 0;
}

//Transfer stack pointer to x register
uint8_t olc6502::TSX() {
  x = stkp;
  SetFlag(N, x & 0x80);
  SetFlag(Z, x == 0x00);
  return 0;
}

//Transfer register x to accumulator
uint8_t olc6502::TXA() {
  a = x;
  SetFlag(N, a & 0x80);
  SetFlag(Z, a == 0x00);
  return 0;
}

//Transfer register x to stack pointer
uint8_t olc6502::TXS() {
  stkp = x;
  return 0;
}

//Transfer register y to stack pointer
uint8_t olc6502::TYA()
{
  a = y;
  SetFlag(Z, a == 0x00);
  SetFlag(N, a & 0x80);
  return 0;
}

//Illegal opcodes
uint8_t olc6502::XXX()
{
  return 0;
}


//--------------------------------------------
//Debbugging functions
bool olc6502::complete()
{
  return cycles == 0;
}

std::map<uint16_t, std::string> olc6502::disassemble(uint16_t nStart, uint16_t nStop)
{
  uint32_t addr = nStart;
  uint32_t value = 0x00, lo=0x00, hi=0x00;
  std::map<uint16_t, std::string> mapLines;
  uint16_t line_addr = 0;

  auto hex = [](uint16_t n, uint16_t d)
  {
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
      s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
  };

  while (addr <= (uint32_t)nStop)
  {
    line_addr = addr;

    // Prefix line with instruction address
    std::string sInst = "$" + hex(addr, 4) + ": ";

    // cpuRead instruction, and get its cpuReadable name
    uint8_t opcode = bus->cpuRead(addr, true); addr++;
    sInst += lookup[opcode].name + " ";

    // Get oprands from desired locations, and form the
    // instruction based upon its addressing mode. These
    // routines mimmick the actual fetch routine of the
    // 6502 in order to get accurate data as part of the
    // instruction
    if (lookup[opcode].addrmode == &olc6502::IMP)
    {
      sInst += " {IMP}";
    }
    else if (lookup[opcode].addrmode == &olc6502::IMM)
    {
      value = bus->cpuRead(addr, true); addr++;
      sInst += "#$" + hex(value, 2) + " {IMM}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ZP0)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + " {ZP0}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ZPX)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + ", X {ZPX}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ZPY)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = 0x00;
      sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
    }
    else if (lookup[opcode].addrmode == &olc6502::IZX)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = 0x00;
      sInst += "($" + hex(lo, 2) + ", X) {IZX}";
    }
    else if (lookup[opcode].addrmode == &olc6502::IZY)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = 0x00;
      sInst += "($" + hex(lo, 2) + "), Y {IZY}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ABS)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = bus->cpuRead(addr, true); addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ABX)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = bus->cpuRead(addr, true); addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
    }
    else if (lookup[opcode].addrmode == &olc6502::ABY)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = bus->cpuRead(addr, true); addr++;
      sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
    }
    else if (lookup[opcode].addrmode == &olc6502::IND)
    {
      lo = bus->cpuRead(addr, true); addr++;
      hi = bus->cpuRead(addr, true); addr++;
      sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
    }
    else if (lookup[opcode].addrmode == &olc6502::REL)
    {
      value = bus->cpuRead(addr, true); addr++;
      sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
    }

    // Add the formed string to a std::map, using the instruction's
    // address as the key. This makes it convenient to look for later
    // as the instructions are variable in length, so a straight up
    // incremental index is not sufficient.
    mapLines[line_addr] = sInst;
  }

  return mapLines;
}
