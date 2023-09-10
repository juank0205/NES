#pragma once
#include <cstdint>

class olc2C02
{
public:
	olc2C02();
	~olc2C02();

	//Communications with the main bus
	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void cpuWrite(uint16_t addr, uint8_t data);


	//Communications with the PPU bus
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void ppuWrite(uint16_t addr, uint8_t data);
};

