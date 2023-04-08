#include "Inky.hpp"
#include "I2CDevice.hpp"
#include "SPIDevice.hpp"
#include "ImageIO.hpp"

#include <gpio-cpp/gpio.hpp>
#include <fmt/format.h>

#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <stdexcept>

//#define DEBUG_SPI
#ifdef DEBUG_SPI
#include <magic_enum.hpp>
#include <iostream>
using namespace magic_enum::ostream_operators;
#endif

static const uint8_t InkyI2CDevice = 0x50;
static const std::string InkySPIDevice = "/dev/spidev0.0";
static const std::string InkyGPIODevice = "/dev/gpiochip0";

enum class InkyGpioPin : int
{
  RESET_PIN = 27,
  BUSY_PIN = 17,
  DC_PIN = 22,
  MOSI_PIN = 10,
  SCLK_PIN = 11,
  CS0_PIN = 8,
};

// Constants for SSD1608 driver IC
enum class InkyCommand : uint8_t
{
  SSD1683_DRIVER_CONTROL = 0x01,
  SSD1683_GATE_VOLTAGE = 0x03,
  SSD1683_SOURCE_VOLTAGE = 0x04,
  SSD1683_DISPLAY_CONTROL = 0x07,
  SSD1683_NON_OVERLAP = 0x0B,
  SSD1683_BOOSTER_SOFT_START = 0x0C,
  SSD1683_GATE_SCAN_START = 0x0F,
  SSD1683_DEEP_SLEEP = 0x10,
  SSD1683_DATA_MODE = 0x11,
  SSD1683_SW_RESET = 0x12,
  SSD1683_TEMP_WRITE = 0x1A,
  SSD1683_TEMP_READ = 0x1B,
  SSD1683_TEMP_CONTROL = 0x18,
  SSD1683_TEMP_LOAD = 0x1A,
  SSD1683_MASTER_ACTIVATE = 0x20,
  SSD1683_DISP_CTRL1 = 0x21,
  SSD1683_DISP_CTRL2 = 0x22,
  SSD1683_WRITE_RAM = 0x24,
  SSD1683_WRITE_ALTRAM = 0x26,
  SSD1683_READ_RAM = 0x25,
  SSD1683_VCOM_SENSE = 0x2B,
  SSD1683_VCOM_DURATION = 0x2C,
  SSD1683_WRITE_VCOM = 0x2C,
  SSD1683_READ_OTP = 0x2D,
  SSD1683_WRITE_LUT = 0x32,
  SSD1683_WRITE_DUMMY = 0x3A,
  SSD1683_WRITE_GATELINE = 0x3B,
  SSD1683_WRITE_BORDER = 0x3C,
  SSD1683_SET_RAMXPOS = 0x44,
  SSD1683_SET_RAMYPOS = 0x45,
  SSD1683_SET_RAMXCOUNT = 0x4E,
  SSD1683_SET_RAMYCOUNT = 0x4F,

  UC8159_PSR = 0x00,
  UC8159_PWR = 0x01,
  UC8159_POF = 0x02,
  UC8159_PFS = 0x03,
  UC8159_PON = 0x04,
  UC8159_BTST = 0x06,
  UC8159_DSLP = 0x07,
  UC8159_DTM1 = 0x10,
  UC8159_DSP = 0x11,
  UC8159_DRF = 0x12,
  UC8159_IPC = 0x13,
  UC8159_PLL = 0x30,
  UC8159_TSC = 0x40,
  UC8159_TSE = 0x41,
  UC8159_TSW = 0x42,
  UC8159_TSR = 0x43,
  UC8159_CDI = 0x50,
  UC8159_LPD = 0x51,
  UC8159_TCON = 0x60,
  UC8159_TRES = 0x61,
  UC8159_DAM = 0x65,
  UC8159_REV = 0x70,
  UC8159_FLG = 0x71,
  UC8159_AMV = 0x80,
  UC8159_VV = 0x81,
  UC8159_VDCS = 0x82,
  UC8159_PWS = 0xE3,
  UC8159_TSSET = 0xE5,

  NOP = 0xFF,
};

Inky::~Inky() {}

class InkyBase : public Inky
{
protected:
  std::mutex mutex_;
  DisplayInfo info_;
  IndexedColor border_;
  Image buf_;
  IndexedColorMap colorMap_;
  Gpio gpio_;
  SPIDevice spi_;

  InkyBase(DisplayInfo info, uint32_t spiSpeedHz = 488000, uint32_t spiTransferSizeBytes = 4096, SPIMode spiMode = SPIMode::SPI_MODE_0); 

  virtual void setImage(const Image& image, ScaleSettings scale, DitherSettings dither) override;
  virtual Image getImage() const override;
  virtual void setBorder(IndexedColor color) override;
  virtual const DisplayInfo& info() const override;
  virtual const IndexedColorMap& getColorMap() const override;

  void sendCommand(InkyCommand command);
  template <typename T> void sendCommand(InkyCommand command, const T& data);
  static void generatePackedIndex(const Image& img, std::vector<uint8_t>& packed);
  static void generatePackedPlane(const Image& img, std::vector<uint8_t>& packed, IndexedColor color);
  static void sleep(double milliseconds);
  Image generateColorTest();
};

InkyBase::InkyBase(DisplayInfo displayInfo, uint32_t spiSpeedHz, uint32_t spiTransferSizeBytes, SPIMode spiMode) : 
  info_(displayInfo),
  gpio_(InkyGPIODevice),
  spi_(InkySPIDevice, spiSpeedHz, spiTransferSizeBytes, spiMode)
{
  std::vector<std::tuple<ColorName,IndexedColor,RGBAColor>> displayColors;
  switch (info_.colorCapability)
  {
    case ColorCapability::BlackWhite:
      displayColors = 
      {
        {ColorName::White, 0, {255,255,255}},
        {ColorName::Black, 1, {0,0,0}}
      };
      break;
    case ColorCapability::BlackWhiteRed:
      displayColors = 
      {
        {ColorName::White, 0, {255,255,255}},
        {ColorName::Black, 1, {0,0,0}},
        {ColorName::Red, 2, {255,0,0}}
      };
      break;
    case ColorCapability::BlackWhiteYellow:
      displayColors = 
      {
        {ColorName::White, 0, {255,255,255}},
        {ColorName::Black, 1, {0,0,0}},
        {ColorName::Red, 2, {255,0,0}}
      };
      break;
    case ColorCapability::SevenColor:
    case ColorCapability::SevenColorv2:
      displayColors = 
        {
          {ColorName::Black, 0, {48, 45, 72}},
          {ColorName::White, 1, {204, 194, 184}},
          {ColorName::Green, 2, {71, 98, 73}},
          {ColorName::Blue, 3, {81, 71, 107}},
          {ColorName::Red, 4, {167, 73, 69}},
          {ColorName::Yellow, 5, {214, 180, 90}},
          {ColorName::Orange, 6, {200, 121, 91}},

           //{ColorName::Black, 0, {0,0,0}},
           //{ColorName::White, 1, {255,255,255}},
          // {ColorName::Green, 2, {0,255,0}},
          // {ColorName::Blue, 3, {0,0,255}},
          // {ColorName::Red, 4, {255,0,0}},
          // {ColorName::Yellow, 5, {255,255,0}},
          // {ColorName::Orange, 6, {255,127,0}},
        };
      break;
  }
  colorMap_ = IndexedColorMap(displayColors);
  //colorMap_.normalizePaletteByLab(false, true);
  border_ = colorMap_.toIndexedColor(ColorName::White);
  buf_ = Image(info_.width, info_.height, colorMap_);
}

const IndexedColorMap& InkyBase::getColorMap() const
{
  return colorMap_;
}

void InkyBase::sendCommand(InkyCommand command)
{
  gpio_.write(InkyGpioPin::DC_PIN, false);
  #ifdef DEBUG_SPI
  std::cout << "Command " << command << " ret: " << 
  #endif
  spi_.write((uint8_t*)(&command), 1);
  #ifdef DEBUG_SPI
  std::cout << std::endl;
  #endif
}

template <typename T>
void InkyBase::sendCommand(InkyCommand command, const T& data)
{
  sendCommand(command);

  size_t len = 0;
  const uint8_t* dataPtr = nullptr;

  if constexpr(std::is_same<T, std::initializer_list<uint8_t>>())
  {
    len = data.size();
    dataPtr = std::data(data);
  }
  else if constexpr(std::is_same<T, std::vector<uint8_t>>())
  {
    len = data.size();
    dataPtr = data.data();
  }
  else if constexpr(std::is_arithmetic<T>())
  {
    len = sizeof(T);
    dataPtr = (uint8_t*)(&data);
  }
  else if constexpr(std::is_trivial<T>() && std::is_standard_layout<T>())
  {
    len = sizeof(T);
    dataPtr = (uint8_t*)(&data);
  }
  else
  {
    static_assert(std::is_trivial<T>() && std::is_standard_layout<T>(), "Unsupported data type!");
  }

  gpio_.write(InkyGpioPin::DC_PIN, true);
  #ifdef DEBUG_SPI
  std::cout << "Sent buffer len " << len << " ret: " << 
  #endif
  spi_.write(dataPtr, len);
  #ifdef DEBUG_SPI
  std::cout << std::endl;
  #endif
}

const Inky::DisplayInfo& InkyBase::info() const 
{
  return info_;
}

void InkyBase::setImage(const Image& image, ScaleSettings scale, DitherSettings dither)
{
  std::lock_guard lock(mutex_);
  buf_ = image;
  buf_.scale(info_.width, info_.height, scale);
  buf_.toIndexed(colorMap_, dither);
}

Image InkyBase::getImage() const
{
  return buf_;
}

void InkyBase::setBorder(IndexedColor inky)
{
  std::lock_guard lock(mutex_);
  border_ = inky;
}

void InkyBase::generatePackedIndex(const Image& img, std::vector<uint8_t>& packed)
{
  // Pack the image buffer so that each value is 4 bytes, 2 pixels per byte
  // buf = ((buf[::2] << 4) & 0xF0) | (buf[1::2] & 0x0F)
  packed.resize(img.width() * img.height() / 2);
  const IndexedColor* bufData = (IndexedColor*)img.data();
  for (int i = 0; i < packed.size(); ++i)
  {
    packed[i] = (uint8_t)((bufData[i*2] << 4) | (bufData[i*2+1] & 0x0F));
  }
}

void InkyBase::generatePackedPlane(const Image& img, std::vector<uint8_t>& packed, IndexedColor color)
{
  int size = (img.width()*img.height());
  int wholeBytes = size/8;
  int packedSize = size/8 + ((size%8 != 0) ? 1 : 0);
  packed.resize(packedSize);
  const IndexedColor* inkyData = (const IndexedColor*)img.data();
  for (int i = 0; i < wholeBytes; ++i)
  {
    packed[i] = ((inkyData[i*8+0] == color) ? (uint8_t)0b10000000 : (uint8_t)0) | 
                ((inkyData[i*8+1] == color) ? (uint8_t)0b01000000 : (uint8_t)0) |
                ((inkyData[i*8+2] == color) ? (uint8_t)0b00100000 : (uint8_t)0) |
                ((inkyData[i*8+3] == color) ? (uint8_t)0b00010000 : (uint8_t)0) |
                ((inkyData[i*8+4] == color) ? (uint8_t)0b00001000 : (uint8_t)0) |
                ((inkyData[i*8+5] == color) ? (uint8_t)0b00000100 : (uint8_t)0) |
                ((inkyData[i*8+6] == color) ? (uint8_t)0b00000010 : (uint8_t)0) |
                ((inkyData[i*8+7] == color) ? (uint8_t)0b00000001 : (uint8_t)0);
  }

  // Get that sneaky final byte and write to it
  if (packedSize > wholeBytes)
  {
    packed[wholeBytes] = 0;
    if (wholeBytes*8+0 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+0] == color) ? (uint8_t)0b00000001 : (uint8_t)0); 
    if (wholeBytes*8+1 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+1] == color) ? (uint8_t)0b00000010 : (uint8_t)0);
    if (wholeBytes*8+2 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+2] == color) ? (uint8_t)0b00000100 : (uint8_t)0);
    if (wholeBytes*8+3 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+3] == color) ? (uint8_t)0b00001000 : (uint8_t)0);
    if (wholeBytes*8+4 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+4] == color) ? (uint8_t)0b00010000 : (uint8_t)0);
    if (wholeBytes*8+5 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+5] == color) ? (uint8_t)0b00100000 : (uint8_t)0);
    if (wholeBytes*8+6 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+6] == color) ? (uint8_t)0b01000000 : (uint8_t)0);
    if (wholeBytes*8+7 < size) packed[wholeBytes] |= ((inkyData[wholeBytes*8+7] == color) ? (uint8_t)0b10000000 : (uint8_t)0);
  }
}

void InkyBase::sleep(double milliseconds)
{
  if (milliseconds > 0.0)
  {
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(milliseconds));
  }
}

static inline int64_t millisecondsSinceEpoch()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

Image InkyBase::generateColorTest()
{
  Image colorTest(info_.width, info_.height, colorMap_);
  auto data = colorTest.data();
  const auto& indexedColors = colorMap_.indexedColors();
  int colsPerColor = info_.width / indexedColors.size();
  for (int i = 0; i < info_.width*info_.height; ++i)
  {
    int x = i % info_.width;
    data[i] = indexedColors[std::clamp(x / colsPerColor, 0, (int)(indexedColors.size()-1))];
  }
  return colorTest;
}

class SimulatedInky : public InkyBase
{
  public:
  SimulatedInky();
  virtual void show(ShowOperation op) override;
};

SimulatedInky::SimulatedInky() : InkyBase(
  {
    // Some fake data on non-pi platforms
    .width = 640,
    .height = 400,
    .colorCapability = ColorCapability::SevenColor,
    .pcbVariant = 12,
    .displayVariant = DisplayVariant::Seven_Colour_640x400_UC8159,
    .writeTime = "2022-09-02 11:54:06.4"
  }, 0
) {}

void SimulatedInky::show(ShowOperation op)
{
  std::lock_guard lock(mutex_);
  // Write the images to display RAM
  if (op == ShowOperation::BufferedImage)
  {
    ImageIO::SaveToFile(fmt::format("Inky_{}.png", millisecondsSinceEpoch()), buf_, {.saveFormat = ImageFormat::PNG});
  }
  else if (op == ShowOperation::ColorTest)
  {
    Image colorTest = generateColorTest();
    ImageIO::SaveToFile(fmt::format("Inky_{}.png", millisecondsSinceEpoch()), colorTest, {.saveFormat = ImageFormat::PNG});
  }
}

class InkySSD1683 final : public InkyBase
{
  private: 
  static const int SPIDeviceSpeedHz = 10000000;
  std::vector<uint8_t> whitePlane;
  std::vector<uint8_t> colorPlane;
  void reset();
  void waitForBusy(int timeoutMs = 5000);
  public:
  InkySSD1683(DisplayInfo info);
  virtual void show(ShowOperation op) override;
};

InkySSD1683::InkySSD1683(DisplayInfo info) : InkyBase(info, SPIDeviceSpeedHz)
{
  if (info.displayVariant != DisplayVariant::Black_wHAT_SSD1683 &&
      info.displayVariant != DisplayVariant::Red_wHAT_SSD1683 &&
      info.displayVariant != DisplayVariant::Yellow_wHAT_SSD1683)
  {
    throw std::runtime_error("Unsupported Inky display type!!");
  }

  // Setup the GPIO pins
  gpio_.setupLine(InkyGpioPin::DC_PIN, Gpio::LineMode::Output);
  gpio_.write(InkyGpioPin::DC_PIN, false);
  gpio_.setupLine(InkyGpioPin::RESET_PIN, Gpio::LineMode::Output);
  gpio_.write(InkyGpioPin::RESET_PIN, true);
  gpio_.setupLine(InkyGpioPin::BUSY_PIN, Gpio::LineMode::Input);
}

void InkySSD1683::reset()
{
  // Perform a hardware reset
  gpio_.write(InkyGpioPin::RESET_PIN, false);
  sleep(500);
  gpio_.write(InkyGpioPin::RESET_PIN, true);
  sleep(500);
  sendCommand(InkyCommand::SSD1683_SW_RESET);
  sleep(1000);
  waitForBusy();
}

void InkySSD1683::waitForBusy(int timeoutMs)
{
  int i = 0;
  while (gpio_.read(InkyGpioPin::BUSY_PIN) != 0)
  {
    sleep(10);
    ++i;
    if (i*10 > timeoutMs)
    {
      throw std::runtime_error("Timed out while wating for display to finish an operation.");
    }
  }
}

void InkySSD1683::show(ShowOperation op)
{
  std::lock_guard lock(mutex_);

  reset();

  sendCommand(InkyCommand::SSD1683_DRIVER_CONTROL, (uint8_t[]){(uint8_t)(info_.height - 1), (uint8_t)((info_.height - 1) >> 8), 0x00});
  // Set dummy line period
  sendCommand(InkyCommand::SSD1683_WRITE_DUMMY, uint8_t{0x1B});
  // Set Line Width
  sendCommand(InkyCommand::SSD1683_WRITE_GATELINE, uint8_t{0x0B});
  // Data entry squence (scan direction leftward and downward)
  sendCommand(InkyCommand::SSD1683_DATA_MODE, uint8_t{0x03});
  // Set ram X start and end position
  sendCommand(InkyCommand::SSD1683_SET_RAMXPOS, (uint8_t[]){(uint8_t)0x00, (uint8_t)((info_.width / 8) - 1)});
  // Set ram Y start and end position
  sendCommand(InkyCommand::SSD1683_SET_RAMYPOS, (uint8_t[]){0x00, 0x00, (uint8_t)(info_.height - 1), (uint8_t)((info_.height - 1) >> 8)});
  // VCOM Voltage
  sendCommand(InkyCommand::SSD1683_WRITE_VCOM, uint8_t{0x70});
  // Write LUT DATA
  // sendCommand(InkyCommand::WRITE_LUT, self._luts[self.lut])

  if (border_ == colorMap_.toIndexedColor(ColorName::Black))
  {
    sendCommand(InkyCommand::SSD1683_WRITE_BORDER, uint8_t{0b00000000});
    // GS Transition + Waveform 00 + GSA 0 + GSB 0
  }  
  else if (border_ == colorMap_.toIndexedColor(ColorName::Red))
  {
    sendCommand(InkyCommand::SSD1683_WRITE_BORDER, uint8_t{0b00000110});
    // GS Transition + Waveform 01 + GSA 1 + GSB 0
  }
  else if (border_ == colorMap_.toIndexedColor(ColorName::Yellow))
  {
    sendCommand(InkyCommand::SSD1683_WRITE_BORDER, uint8_t{0b00001111});
    // GS Transition + Waveform 11 + GSA 1 + GSB 1
  }
  else if (border_ == colorMap_.toIndexedColor(ColorName::White))
  {
    sendCommand(InkyCommand::SSD1683_WRITE_BORDER, uint8_t{0b00000001});
    // GS Transition + Waveform 00 + GSA 0 + GSB 1
  }

  // Set RAM address to 0, 0
  sendCommand(InkyCommand::SSD1683_SET_RAMXCOUNT, uint8_t{0x00});
  sendCommand(InkyCommand::SSD1683_SET_RAMYCOUNT, (uint8_t[2]){0x00, 0x00});

  // Write the images to display RAM
  if (op == ShowOperation::BufferedImage)
  {
    generatePackedPlane(buf_, whitePlane, colorMap_.toIndexedColor(ColorName::White));
    
    if (info_.colorCapability == ColorCapability::BlackWhiteRed)
    {
      generatePackedPlane(buf_, colorPlane, colorMap_.toIndexedColor(ColorName::Red));
    }
    else if (info_.colorCapability == ColorCapability::BlackWhiteYellow)
    {
      generatePackedPlane(buf_, colorPlane, colorMap_.toIndexedColor(ColorName::Yellow));
      
    }
  }
  else if (op == ShowOperation::ColorTest)
  {
    Image colorTest = generateColorTest();
    generatePackedPlane(colorTest, whitePlane, colorMap_.toIndexedColor(ColorName::White));

    if (info_.colorCapability == ColorCapability::BlackWhiteRed)
    {
      generatePackedPlane(colorTest, colorPlane, colorMap_.toIndexedColor(ColorName::Red));
    }
    else if (info_.colorCapability == ColorCapability::BlackWhiteYellow)
    {
      generatePackedPlane(colorTest, colorPlane, colorMap_.toIndexedColor(ColorName::Yellow));
    }
  }
  
  sendCommand(InkyCommand::SSD1683_WRITE_RAM, whitePlane);
  sendCommand(InkyCommand::SSD1683_WRITE_ALTRAM, colorPlane);

  waitForBusy();
  sendCommand(InkyCommand::SSD1683_MASTER_ACTIVATE);
}

class InkyUC8159 final : public InkyBase
{
  private:
  struct CorrectionData
  {
    int cols = 0;
    int rows = 0;
    uint8_t rotation = 0;
    uint8_t offsetX = 0;
    uint8_t offsetY = 0;
    uint8_t resolutionSetting = 0;
  };
  static const uint32_t DefaultSPIDeviceSpeedHz = 3000000;
  static const uint32_t DefaultSPITransferSize = 4096;
  static const SPIMode DefaultSPIMode = SPIMode::SPI_MODE_0; //SPIMode::SPI_NO_CS;
  CorrectionData correctionData;
  std::vector<uint8_t> packed;
  void reset();
  void waitForBusy(int timeoutMs = 40000);
  public:
  InkyUC8159(DisplayInfo info);
  virtual void show(ShowOperation op) override;
};

InkyUC8159::InkyUC8159(DisplayInfo info) : InkyBase(info, DefaultSPIDeviceSpeedHz, DefaultSPITransferSize, DefaultSPIMode)
{
  // Detect the display type, make sure it's supported, and set some correction data
  switch (info.displayVariant)
  {
    case DisplayVariant::Seven_Colour_UC8159:
      correctionData = 
      { 
        .cols = 600,
        .rows = 448,
        .resolutionSetting = 0b11000000,
      };
    break;
    case DisplayVariant::Seven_Colour_640x400_UC8159:
    case DisplayVariant::Seven_Colour_640x400_UC8159_v2:
      correctionData = 
      { 
        .cols = 640,
        .rows = 400,
        .resolutionSetting = 0b10000000,
      };
    break;
    default:
      throw std::runtime_error("Unsupported Inky display type!!");
  }

  // Correct the eeprom and buffer sizes
  info_.width = correctionData.cols;
  info_.height = correctionData.rows;
  buf_ = Image(info_.width, info_.height, colorMap_);

  // Setup the GPIO pins
  gpio_.setupLine(InkyGpioPin::DC_PIN, Gpio::LineMode::Output);
  gpio_.write(InkyGpioPin::DC_PIN, false);
  gpio_.setupLine(InkyGpioPin::RESET_PIN, Gpio::LineMode::Output);
  gpio_.write(InkyGpioPin::RESET_PIN, true);
  gpio_.setupLine(InkyGpioPin::BUSY_PIN, Gpio::LineMode::Input);
}

void InkyUC8159::reset()
{
    gpio_.write(InkyGpioPin::RESET_PIN, false);
    sleep(100);
    gpio_.write(InkyGpioPin::RESET_PIN, true);

    waitForBusy(1000);

    // Resolution Setting
    // 10bit horizontal followed by a 10bit vertical resolution
    sendCommand(InkyCommand::UC8159_TRES, (uint16_t[]){info_.width, info_.height} );

    // Panel Setting
    // 0b11000000 = Resolution select, 0b00 = 640x480, our panel is 0b11 = 600x448
    // 0b00100000 = LUT selection, 0 = ext flash, 1 = registers, we use ext flash
    // 0b00010000 = Ignore
    // 0b00001000 = Gate scan direction, 0 = down, 1 = up (default)
    // 0b00000100 = Source shift direction, 0 = left, 1 = right (default)
    // 0b00000010 = DC-DC converter, 0 = off, 1 = on
    // 0b00000001 = Soft reset, 0 = Reset, 1 = Normal (Default)
    // 0b11 = 600x448
    // 0b10 = 640x400
    sendCommand(InkyCommand::UC8159_PSR, (uint8_t[])
    {
        (uint8_t)(correctionData.resolutionSetting | 0b00101111),  // See above for more magic numbers
        0x08                                                       // display_colours == UC8159_7C
    });

    // Power Settings
    sendCommand(InkyCommand::UC8159_PWR, (uint8_t[])
    {
        (0x06 << 3) |  // ??? - not documented in UC8159 datasheet  # noqa: W504
        (0x01 << 2) |  // SOURCE_INTERNAL_DC_DC                     # noqa: W504
        (0x01 << 1) |  // GATE_INTERNAL_DC_DC                       # noqa: W504
        (0x01),        // LV_SOURCE_INTERNAL_DC_DC
        0x00,          // VGx_20V
        0x23,          // UC8159_7C
        0x23           // UC8159_7C
    });

    // Set the PLL clock frequency to 50Hz
    // 0b11000000 = Ignore
    // 0b00111000 = M
    // 0b00000111 = N
    // PLL = 2MHz * (M / N)
    // PLL = 2MHz * (7 / 4)
    // PLL = 2,800,000 ???
    sendCommand(InkyCommand::UC8159_PLL, (uint8_t)0x3C);  // 0b00111100

    // Send the TSE register to the display
    sendCommand(InkyCommand::UC8159_TSE, (uint8_t)0x00);  // Colour

    // VCOM and Data Interval setting
    // 0b11100000 = Vborder control (0b001 = LUTB voltage)
    // 0b00010000 = Data polarity
    // 0b00001111 = Vcom and data interval (0b0111 = 10, default)
    sendCommand(InkyCommand::UC8159_CDI, (uint8_t)((border_ << 5) | 0x17));  // 0b00110111

    // Gate/Source non-overlap period
    // 0b11110000 = Source to Gate (0b0010 = 12nS, default)
    // 0b00001111 = Gate to Source
    sendCommand(InkyCommand::UC8159_TCON, (uint8_t)0x22);  // 0b00100010

    // Disable external flash
    sendCommand(InkyCommand::UC8159_DAM, (uint8_t)0x00);

    // UC8159_7C
    sendCommand(InkyCommand::UC8159_PWS, (uint8_t)0xAA);

    // Power off sequence
    // 0b00110000 = power off sequence of VDH and VDL, 0b00 = 1 frame (default)
    // All other bits ignored?
    sendCommand(InkyCommand::UC8159_PFS, (uint8_t)0x00);  // PFS_1_FRAME
}

void InkyUC8159::waitForBusy(int timeoutMs)
{
  // If the busy_pin is *high* (pulled up by host)
  // then assume we're not getting a signal from inky
  // and wait the timeout period to be safe.
  if (gpio_.read(InkyGpioPin::BUSY_PIN) == 1)
  {
    sleep(timeoutMs);
    return;
  }

  int i = 0;
  while (gpio_.read(InkyGpioPin::BUSY_PIN) == 0)
  {
    sleep(10);
    ++i;
    if (i*10 > timeoutMs)
    {
      throw std::runtime_error("Timed out while wating for display to finish an operation.");
    }
  }
}

void InkyUC8159::show(ShowOperation op)
{
  std::lock_guard lock(mutex_);

  if (op == ShowOperation::BufferedImage)
  {
    generatePackedIndex(buf_, packed);
  }
  else if (op == ShowOperation::ColorTest)
  {
    generatePackedIndex(generateColorTest(), packed);
  }
  else if (op == ShowOperation::CleanDisplay)
  {
    Image cleanImage(info_.width, info_.height, colorMap_);
    for (int i=0; i < info_.width*info_.height; ++i)
    {
      cleanImage.data()[i] = (uint8_t)7;
    }
    generatePackedIndex(cleanImage, packed);
  }

  reset();

  sendCommand(InkyCommand::UC8159_DTM1, packed);

  sendCommand(InkyCommand::UC8159_PON);
  waitForBusy(200);

  sendCommand(InkyCommand::UC8159_DRF);
  waitForBusy(32000);

  sendCommand(InkyCommand::UC8159_POF);
  waitForBusy(200);
}

static uint16_t read16(const uint8_t* buf)
{
  return ((uint16_t)buf[0]) | ((uint16_t)buf[1] << 8);
}

static void write16(uint16_t val, uint8_t* buf)
{
  buf[0] = (uint8_t) val;
  buf[1] = (uint8_t) (val >> 8);
}

Inky::DisplayInfo readEeprom()
{
  I2CDevice eeprom(InkyI2CDevice);
  // eeprom format is python struct '<HHBBB22p'
  // width, height, color, pcb_variant, display_variant, write_time
  uint8_t rom[30];
  eeprom.read(0, rom, 29);

  // Get the length of the writeTime string and null terminate it
  // The data could lie about length, so cap it to 21
  uint8_t len = std::min(rom[7],(uint8_t)21); 
  rom[8+len] = '\0';

  return
  {
    .width = read16(rom),
    .height = read16(rom+2),
    .colorCapability = (Inky::ColorCapability)rom[4],
    .pcbVariant = rom[5],
    .displayVariant = (Inky::DisplayVariant)rom[6],
    .writeTime = std::string((const char*)rom+8)
  };
}

std::unique_ptr<Inky> Inky::Create()
{
  #ifdef SIMULATE_PI_HARDWARE
  return std::make_unique<SimulatedInky>();
  #else
  auto displayInfo = readEeprom();
  switch (displayInfo.displayVariant)
  {
    case DisplayVariant::Black_wHAT_SSD1683:
    case DisplayVariant::Red_wHAT_SSD1683:
    case DisplayVariant::Yellow_wHAT_SSD1683:
      return std::make_unique<InkySSD1683>(displayInfo);
    case DisplayVariant::Seven_Colour_UC8159:
    case DisplayVariant::Seven_Colour_640x400_UC8159:
    case DisplayVariant::Seven_Colour_640x400_UC8159_v2:
      return std::make_unique<InkyUC8159>(displayInfo);
  }
  throw std::runtime_error("The connected Inky is unsupported!");
  #endif
}