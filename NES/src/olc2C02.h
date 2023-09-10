#pragma once
#include "Cartridge.h"

#include <memory>
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

	//Interface
	void connectCarridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();

private:
	std::shared_ptr<Cartridge> cart;

	uint8_t tblName[2][1024];
	uint8_t tblPallete[32];
	//uint8_t tblPattern[2][4096];
};

