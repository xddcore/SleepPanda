#pragma once

#include <string>
#include <vector>

enum class SPIMode : uint8_t
{
  SPI_CPHA = 0x01,
  SPI_CPOL = 0x02,
  SPI_MODE_0 = (0|0),
  SPI_MODE_1 = (0|SPI_CPHA),
  SPI_MODE_2 = (SPI_CPOL|0),
  SPI_MODE_3 = (SPI_CPOL|SPI_CPHA),
  SPI_CS_HIGH = 0x04,
  SPI_LSB_FIRST = 0x08,
  SPI_3WIRE = 0x10,
  SPI_LOOP = 0x20,
  SPI_NO_CS = 0x40,
  SPI_READY = 0x80,
};

inline SPIMode operator|(SPIMode lhs, SPIMode rhs) 
{
  return static_cast<SPIMode>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

class SPIDevice
{
public:
  SPIDevice(std::string spiDeviceName = "/dev/spidev0.0", uint32_t maxBusSpeedHz = 488000, uint32_t maxTransferSizeBytes = 4096, SPIMode mode = SPIMode::SPI_MODE_0);
  virtual ~SPIDevice();
  // Write the data in buf to the provided address 
  int write(const std::vector<uint8_t>& buf = std::vector<uint8_t>(), uint16_t delayUs = 0);
  // Write the data in buf to the provided address
  int write(const uint8_t* buf, size_t len, uint16_t delayUs = 0);
  // Poke the provided address, wait, then fill buf with the returned data
  int read(std::vector<uint8_t>& buf, uint16_t delayUs = 0);
  // Poke the provided address, wait, then read len bytes into buf
  int read(uint8_t* buf, size_t len, uint16_t delayUs = 0);
private:
  int spiFile_ = -1;
  SPIMode spiMode_ = SPIMode::SPI_MODE_0;
  uint8_t bitsPerWord_ = 8;
  uint32_t maxBusSpeedHz_ = 488000;
  uint32_t maxTransferSizeBytes_ = 4096;
};
