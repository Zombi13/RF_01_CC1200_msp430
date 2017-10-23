// Address Config = No address check 
// Base Frequency = 869.524780 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 869.524780 
// Channel Number = 0 
// Channel Spacing = 207.641602 
// Data Format = Normal mode 
// Data Rate = 4.80223 
// Deviation = 24.719238 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = GFSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 105.468750 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 20 
// Whitening = false 

#include "../../../mcu_lib/usci_b_spi.h"
#include "../cc1101_def.h"

static const registerSetting_t preferredSettings[]= 
{
  {0x0000,    0x5C},    //GDO2 Output Pin Configuration
  {0x0002,    0x06},    //GDO0 Output Pin Configuration
  {0x0003,    0xC7},
  {0x0008,    0x05},
  {0x000B,    0x06},
  {0x000D,    0x20},
  {0x000E,    0x34},
  {0x000F,    0x62},
  {0x0010,    0xC7},
  {0x0011,    0x75},
  {0x0012,    0x13},
  {0x0015,    0x37},
  {0x0018,    0x18},
  {0x0019,    0x16},
  {0x001B,    0x43},
  {0x0020,    0xFB},
  {0x0023,    0xE9},
  {0x0024,    0x2A},
  {0x0025,    0x00},
  {0x0026,    0x1F},
  {0x002C,    0x81},
  {0x002D,    0x2D},
  {0x002E,    0x09},

  {0x003E,    0x03}, // PA_power
  {0x0003,    0xF7}, // LNA_power
};
