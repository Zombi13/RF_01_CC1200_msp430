// Address Config = No address check 
// Bit Rate = 38.4 
// Carrier Frequency = 920.000000 
// Deviation = 20.019531 
// Device Address = 0 
// Manchester Enable = false 
// Modulation Format = 2-GFSK 
// PA Ramping = true 
// Packet Bit Length = 0 
// Packet Length = 255 
// Packet Length Mode = Variable 
// Performance Mode = High Performance 
// RX Filter BW = 100.000000 
// Symbol rate = 38.4 
// TX Power = 17 
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
 {CC120X_IOCFG3,            0x06},  //int  //GPIO3 IO Pin Configuration
 {CC120X_IOCFG2,            0x58},  //LNA  //GPIO2 IO Pin Configuration
 {CC120X_IOCFG0,            0x59},  //PA  //GPIO0 IO Pin Configuration

  {0x0002,    0xB0},

  {0x0008,    0x08},
  {0x000A,    0x48},
  {0x000B,    0x0D},
  {0x000C,    0x1C},
  {0x0010,    0x00},
  {0x0011,    0x02},
  {0x0013,    0x05},
  {0x0014,    0x93},
  {0x0018,    0x19},
  {0x001C,    0xA9},
  {0x001D,    0xCF},
  {0x001E,    0x00},
  {0x0021,    0x12},
  {0x0028,    0x20},
  {0x002B,    0x55},
  {0x002D,    0x7B},
  {0x002E,    0xFF},
  {0x2F00,    0x00},
  {0x2F01,    0x22},
  {0x2F0C,    0x73},
  {0x2F12,    0x00},
  {0x2F13,    0x5F},
  {0x2F16,    0x40},
  {0x2F17,    0x0E},
  {0x2F19,    0x03},
  {0x2F1B,    0x33},
  {0x2F1D,    0x17},
  {0x2F1F,    0x50},
  {0x2F20,    0x6E},
  {0x2F21,    0x14},
  {0x2F22,    0xAC},
  {0x2F27,    0xB4},
  {0x2F32,    0x0E},
  {0x2F36,    0x03},
};
#ifdef  __cplusplus
}
#endif

#endif
