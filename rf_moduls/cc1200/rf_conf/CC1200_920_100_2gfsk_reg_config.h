// Address Config = No address check 
// Bit Rate = 100 
// Carrier Frequency = 920.599976 
// Deviation = 49.896240 
// Device Address = 0 
// Manchester Enable = false 
// Modulation Format = 2-GFSK 
// Packet Bit Length = 0 
// Packet Length = 255 
// Packet Length Mode = Variable 
// RX Filter BW = 208.333333 
// Symbol rate = 100 
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
#include "../../../rf_moduls/cc120x_spi.h"

static const registerSetting_t preferredSettings[]= 
{

 {CC120X_IOCFG3,            0x59}, //0x59 - PA
 {CC120X_IOCFG2,            0x58}, //0x58 - LNA
 {CC120X_IOCFG0,            0x06},
 /*
 {CC120X_IOCFG3,            0x06},  //int  //GPIO3 IO Pin Configuration
 {CC120X_IOCFG2,            0x58},  //LNA  //GPIO2 IO Pin Configuration
 {CC120X_IOCFG0,            0x59},  //PA  //GPIO0 IO Pin Configuration
*/
  {0x0008,    0xA8},
  {0x0009,    0x23},
  {0x000A,    0x47},
  {0x000B,    0x0C},
  {0x000C,    0x4B},
  {0x000E,    0x8A},
  {0x000F,    0xD8},
  {0x0010,    0x08},
  {0x0011,    0x42},
  {0x0012,    0x05},
  {0x0013,    0xA4},
  {0x0014,    0x7A},
  {0x0015,    0xE1},
  {0x0016,    0x2A},
  {0x0017,    0x01},
  {0x001B,    0x12},
  {0x001C,    0x80},
  {0x001D,    0x00},
  {0x0020,    0x12},
  {0x0026,    0x00},
  {0x0028,    0x20},
  {0x002B,    0x55},
  {0x002E,    0xFF},
  {0x2F00,    0x1C},
  {0x2F02,    0x03},
  {0x2F05,    0x02},
  {0x2F0C,    0x5C},
  {0x2F0D,    0x0F},
  {0x2F0E,    0x5C},
  {0x2F10,    0xEE},
  {0x2F11,    0x10},
  {0x2F12,    0x04},
  {0x2F13,    0x55},
  {0x2F16,    0x40},
  {0x2F17,    0x0E},
  {0x2F19,    0x03},
  {0x2F1B,    0x33},
  {0x2F1D,    0x17},
  {0x2F1F,    0x00},
  {0x2F20,    0x6E},
  {0x2F21,    0x1C},
  {0x2F22,    0xAC},
  {0x2F27,    0xB5},
  {0x2F2F,    0x09},
  {0x2F32,    0x0E},
  {0x2F36,    0x03},
};
#ifdef  __cplusplus
}
#endif

#endif
