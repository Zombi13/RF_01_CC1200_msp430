// Address Config = No address check 
// Bit Rate = 50 
// Carrier Frequency = 867.999878 
// Deviation = 24.948120 
// Device Address = 0 
// Manchester Enable = false 
// Modulation Format = 2-FSK 
// Packet Bit Length = 0 
// Packet Length = 255 
// Packet Length Mode = Variable 
// RX Filter BW = 104.166667 
// Symbol rate = 50 
// Whitening = false 

#include "../../../mcu_lib/usci_b_spi.h"
#include "../cc120x_def.h"

static const registerSetting_t preferredSettings[]= 
{
  {CC120X_IOCFG3,            0x59},  //0  //GPIO3 IO Pin Configuration
  {CC120X_IOCFG2,            0x58},  //1  //GPIO2 IO Pin Configuration
  {CC120X_IOCFG0,            0x06},  //3  //GPIO0 IO Pin Configuration

  {CC120X_SYNC3,             0x55},
  {CC120X_SYNC2,             0x55},
  {CC120X_SYNC1,             0x7A},
  {CC120X_SYNC0,             0x0E},
  {CC120X_SYNC_CFG1,         0x4B},
  {CC120X_SYNC_CFG0,         0x23},
  {CC120X_DEVIATION_M,       0x47},
  {CC120X_DCFILT_CFG,        0x56},
  {CC120X_PREAMBLE_CFG0,     0xBA},
  {CC120X_IQIC,              0xC8},
  {CC120X_CHAN_BW,           0x84},
  {CC120X_MDMCFG1,           0x40},
  {CC120X_MDMCFG0,           0x05},
  {CC120X_SYMBOL_RATE2,      0x94},
  {CC120X_SYMBOL_RATE1,      0x7A},
  {CC120X_SYMBOL_RATE0,      0xE1},
  {CC120X_AGC_REF,           0x3E},
  {CC120X_AGC_CS_THR,        0xF1},
  {CC120X_AGC_CFG1,          0x11},
  {CC120X_AGC_CFG0,          0x90},
  {CC120X_FS_CFG,            0x12},
  {CC120X_PKT_CFG2,          0x00},
  {CC120X_PKT_CFG0,          0x20},
  {CC120X_PA_CFG1,           0x77},
  {CC120X_PKT_LEN,           0xFF},
  {CC120X_IF_MIX_CFG,        0x18},
  {CC120X_TOC_CFG,           0x03},
  {CC120X_MDMCFG2,           0x02},
  {CC120X_FREQ2,             0x56},
  {CC120X_FREQ1,             0xCC},
  {CC120X_FREQ0,             0xCC},
  {CC120X_IF_ADC1,           0xEE},
  {CC120X_IF_ADC0,           0x10},
  {CC120X_FS_DIG1,           0x07},
  {CC120X_FS_DIG0,           0xAA},
  {CC120X_FS_CAL1,           0x40},
  {CC120X_FS_CAL0,           0x0E},
  {CC120X_FS_DIVTWO,         0x03},
  {CC120X_FS_DSM0,           0x33},
  {CC120X_FS_DVC0,           0x17},
  {CC120X_FS_PFD,            0x00},
  {CC120X_FS_PRE,            0x6E},
  {CC120X_FS_REG_DIV_CML,    0x1C},
  {CC120X_FS_SPARE,          0xAC},
  {CC120X_FS_VCO0,           0xB5},
  {CC120X_IFAMP,             0x05},
  {CC120X_XOSC5,             0x0E},
  {CC120X_XOSC1,             0x03},
};
