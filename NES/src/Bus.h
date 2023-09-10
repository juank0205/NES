#pragma once
#include "olc6502.h"
#include "Cartridge.h"
#include "olc2C02.h"

#include <array>
#include <memory>
#include <cstdint>


class Bus
{
public:
	Bus();
	~Bus();

public: //Devices on the bus
	olc6502 cpu;
	olc2C02 ppu;
	std::array<uint8_t, 2048> cpuRam;
	std::shared_ptr<Cartridge> cart;

public: //cpuRead & cpuWrite
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bcpuReadOnly = false);

public: //System Interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	uint32_t nSystemClockCounter = 0;
};

