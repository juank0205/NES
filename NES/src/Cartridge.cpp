#include "Cartridge.h"


Cartridge::Cartridge(const std::string& sFileName)
{
}

Cartridge::~Cartridge()
{
}

uint8_t Cartridge::cpuRead(uint16_t addr, bool rdonly)
{
    return 0;
}

void Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t Cartridge::ppuRead(uint16_t addr, bool rdonly)
{
    return 0;
}

void Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
}
