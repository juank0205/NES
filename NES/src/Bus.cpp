#include "Bus.h"

Bus::Bus() {
  // Clear RAM
  for (uint8_t &i : ram)
    i = 0x00;
  cpu.ConnectBus(this);
}

Bus::~Bus() {}

//Write to a memory address
void Bus::write(uint16_t addr, uint8_t data) {
  if (addr >= 0x0000 && addr <= 0xFFFF)
    ram[addr] = data;
}

//Read data from a memory address
uint8_t Bus::read(uint16_t addr, bool bReadOnly) {
  if (addr >= 0x0000 && addr <= 0xFFFF)
    return ram[addr];

  return 0x0000;
}
