#include "olc2C02.h"

olc2C02::olc2C02()
{
}

olc2C02::~olc2C02()
{
}

uint8_t olc2C02::cpuRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;

	return data;
}

void olc2C02::cpuWrite(uint16_t addr, uint8_t data)
{
}

uint8_t olc2C02::ppuRead(uint16_t addr, bool rdonly)
{
	return 0;
}

void olc2C02::ppuWrite(uint16_t addr, uint8_t data)
{
}
