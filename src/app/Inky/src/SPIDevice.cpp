#include "SPIDevice.hpp"

#include <stdexcept>
#include <algorithm>
#ifndef SIMULATE_PI_HARDWARE
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif

SPIDevice::SPIDevice(std::string spiDeviceName, uint32_t maxBusSpeedHz, uint32_t maxTransferSizeBytes, SPIMode spiMode) : 
  spiMode_(spiMode),
  maxBusSpeedHz_(maxBusSpeedHz),
  maxTransferSizeBytes_(maxTransferSizeBytes)
{
  #ifndef SIMULATE_PI_HARDWARE
	spiFile_ = open(spiDeviceName.c_str(), O_RDWR);
	if (spiFile_ < 0)
	{
    spiFile_ = -1;
    throw std::runtime_error("Failed to open the spi bus device");
  }

	int ret = ioctl(spiFile_, SPI_IOC_WR_MODE, (uint8_t*)&spiMode_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to write mode on the spi bus");
  }

  ret = ioctl(spiFile_, SPI_IOC_RD_MODE, (uint8_t*)&spiMode_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to read mode on the spi bus");
  }

	ret = ioctl(spiFile_, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to write bits per word on the spi bus");
  }

	ret = ioctl(spiFile_, SPI_IOC_RD_BITS_PER_WORD, &bitsPerWord_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to read bits per word on the spi bus");
  }

	ret = ioctl(spiFile_, SPI_IOC_WR_MAX_SPEED_HZ, &maxBusSpeedHz_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to write max speed on the spi bus");
  }

	ret = ioctl(spiFile_, SPI_IOC_RD_MAX_SPEED_HZ, &maxBusSpeedHz_);
	if (ret == -1)
  {
    throw std::runtime_error("Failed to read max speed on the spi bus");
  }
  #endif
}

SPIDevice::~SPIDevice()
{
  #ifndef SIMULATE_PI_HARDWARE
  if (spiFile_ != -1)
  {
    close(spiFile_);  
  }
  #endif
}

int SPIDevice::write(const std::vector<uint8_t> &buf, uint16_t delayUs)
{
  return write(buf.data(), buf.size(), delayUs);
}

int SPIDevice::read(std::vector<uint8_t> &buf, uint16_t delayUs)
{
  return read(buf.data(), buf.size(), delayUs);
}

int SPIDevice::write(const uint8_t* buf, size_t len, uint16_t delayUs)
{
  int ret = 0;
  #ifndef SIMULATE_PI_HARDWARE

  uint32_t blocks = len / maxTransferSizeBytes_ + ((len % maxTransferSizeBytes_ != 0) ? 1 : 0);
  std::vector<spi_ioc_transfer> transfers(blocks);

  for (int i=0; i < blocks; i++)
  {
    transfers[i] = 
    {
      .tx_buf = (uint64_t)(buf + i*maxTransferSizeBytes_),
      .len = std::min(maxTransferSizeBytes_, len - i*maxTransferSizeBytes_),
      .speed_hz = maxBusSpeedHz_,
      .delay_usecs = delayUs,
      .bits_per_word = bitsPerWord_,
	  };
  }
  for (int i=0; i < blocks; i++)
  {
    ret += ioctl(spiFile_, SPI_IOC_MESSAGE(1), &transfers[i]);
  }
	
  #endif
  return ret;
}

int SPIDevice::read(uint8_t* buf, size_t len, uint16_t delayUs)
{
  int ret = 0;
  #ifndef SIMULATE_PI_HARDWARE
  uint32_t blocks = len / maxTransferSizeBytes_ + ((len % maxTransferSizeBytes_ != 0) ? 1 : 0);
  std::vector<spi_ioc_transfer> transfers(blocks);

  for (int i=0; i < blocks; i++)
  {
    transfers[i] = 
    {
      .rx_buf = (uint64_t)(buf + i*maxTransferSizeBytes_),
      .len = std::min(maxTransferSizeBytes_, len - i*maxTransferSizeBytes_),
      .speed_hz = maxBusSpeedHz_,
      .delay_usecs = delayUs,
      .bits_per_word = bitsPerWord_,
	  };
  }
	ret = ioctl(spiFile_, SPI_IOC_MESSAGE(blocks), transfers.data());
  #endif
  return ret;
}
