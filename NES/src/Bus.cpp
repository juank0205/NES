#include "Bus.h"

Bus::Bus() {
  // Clear cpuRam
  for (uint8_t &i : cpuRam)
    i = 0x00;
  cpu.ConnectBus(this);
}

Bus::~Bus() {}

//cpuWrite to a memory address
void Bus::cpuWrite(uint16_t addr, uint8_t data) {
  if(cart->cpuWrite(addr, data)){}
  else if (addr >= 0x0000 && addr <= 0x1FFF)
    cpuRam[addr & 0x07FF] = data;
  else if (addr >= 0x2000 && addr <= 0x3FFF)
    ppu.cpuWrite(addr & 0x0007, data);
}

//cpuRead data from a memory address
uint8_t Bus::cpuRead(uint16_t addr, bool bcpuReadOnly) {
  uint8_t data = 0x00;
  if(cart->cpuRead(addr, data)){}
  else if (addr >= 0x0000 && addr <= 0x1FFF)
    data = cpuRam[addr & 0x07FF];
  else if (addr >= 0x2000 && addr <= 0x3FFF)
    ppu.cpuRead(addr & 0x0007, bcpuReadOnly);

  return data;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
  this->cart = cartridge;
  ppu.connectCarridge(cartridge);
}

void Bus::reset()
{
  cpu.reset();
  nSystemClockCounter = 0;
}

void Bus::clock()
{
}
