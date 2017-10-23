// Address Config = No address check 
// Base Frequency = 867.999939 
// CRC Autoflush = false 
// CRC Enable = true 
// Carrier Frequency = 867.999939 
// Channel Number = 0 
// Channel Spacing = 199.951172 
// Data Format = Normal mode 
// Data Rate = 9.59587 
// Deviation = 4.760742 
// Device Address = 0 
// Manchester Enable = false 
// Modulated = true 
// Modulation Format = 2-FSK 
// PA Ramping = false 
// Packet Length = 255 
// Packet Length Mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Preamble Count = 4 
// RX Filter BW = 58.035714 
// Sync Word Qualifier Mode = 30/32 sync word bits detected 
// TX Power = 26 
// Whitening = false 
#ifndef CC120X_EASY_LINK_REG_CONFIG_H
#define CC120X_EASY_LINK_REG_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************************************
 * INCLUDES
 */
#include "../../../mcu_lib/usci_b_spi.h"
#include "../cc120x_spi.h"

static const registerSetting_t preferredSettings[]= 
{
  {0x0002,    0x06},
  {0x0003,    0x47},
  {0x0008,    0x05},
  {0x000B,    0x06},
  {0x000D,    0x21},
  {0x000E,    0x62},
  {0x000F,    0x76},
  {0x0010,    0xF8},
  {0x0011,    0x83},
  {0x0012,    0x03},
  {0x0015,    0x14},
  {0x0018,    0x18},
  {0x0019,    0x16},
  {0x001B,    0x43},
  {0x0020,    0xFB},
  {0x0023,    0xE9},
  {0x0024,    0x2A},
  {0x0025,    0x00},
  {0x0026,    0x1F},
  {0x002C,    0x81},
  {0x002D,    0x35},
  {0x002E,    0x09},

  {0x003E,    0x03}, // PA_power
  {0x0003,    0xF7}, // LNA_power
};
#ifdef  __cplusplus
}
#endif

#endif
