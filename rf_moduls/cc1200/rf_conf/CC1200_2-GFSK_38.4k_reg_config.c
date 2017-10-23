
#include "../../../mcu_lib/usci_b_spi.h"
#include "../cc120x_def.h"


static const registerSetting_t preferredSettings[]= 
{
  {CC120X_IOCFG0,            0x06},
  {CC120X_IOCFG2,            0x58},
  {CC120X_IOCFG3,            0x59},

  {CC120X_SYNC_CFG1,         0xA9},
  {CC120X_MODCFG_DEV_E,      0x0B},
  {CC120X_PREAMBLE_CFG0,     0x8A},
  {CC120X_IQIC,              0xC8},
  {CC120X_CHAN_BW,           0x10},
  {CC120X_MDMCFG1,           0x42},
  {CC120X_MDMCFG0,           0x05},
  {CC120X_SYMBOL_RATE2,      0x8F},
  {CC120X_SYMBOL_RATE1,      0x75},
  {CC120X_SYMBOL_RATE0,      0x10},
  {CC120X_AGC_REF,           0x27},
  {CC120X_AGC_CS_THR,        0xEE},
  {CC120X_AGC_CFG1,          0x11},
  {CC120X_AGC_CFG0,          0x94},
  {CC120X_FIFO_CFG,          0x00},
  {CC120X_FS_CFG,            0x12},
  {CC120X_PKT_CFG2,          0x00},
  {CC120X_PKT_CFG0,          0x20},

  {CC120X_PA_CFG1,           0x77},

  {CC120X_PKT_LEN,           0xFF},
  {CC120X_IF_MIX_CFG,        0x1C},
  {CC120X_TOC_CFG,           0x03},
  {CC120X_MDMCFG2,           0x02},
  {CC120X_FREQ2,             0x56},
  {CC120X_FREQ1,             0xCC},
  {CC120X_FREQ0,             0xCC},
  {CC120X_IF_ADC1,           0xEE},
  {CC120X_IF_ADC0,           0x10},
  {CC120X_FS_DIG1,           0x04},
  {CC120X_FS_DIG0,           0x50},
  {CC120X_FS_CAL1,           0x40},
  {CC120X_FS_CAL0,           0x0E},
  {CC120X_FS_DIVTWO,         0x03},
  {CC120X_FS_DSM0,           0x33},
  {CC120X_FS_DVC1,           0xF7},
  {CC120X_FS_DVC0,           0x0F},
  {CC120X_FS_PFD,            0x00},
  {CC120X_FS_PRE,            0x6E},
  {CC120X_FS_REG_DIV_CML,    0x1C},
  {CC120X_FS_SPARE,          0xAC},
  {CC120X_FS_VCO0,           0xB5},
  {CC120X_IFAMP,             0x09},
  {CC120X_XOSC5,             0x0E},
  {CC120X_XOSC1,             0x03}
};
