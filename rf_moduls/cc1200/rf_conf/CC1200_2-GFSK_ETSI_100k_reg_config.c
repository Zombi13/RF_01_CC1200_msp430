// Address Config = No address check 
// Bit Rate = 100 
// Carrier Frequency = 867.999878 
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

#include "../../../mcu_lib/usci_b_spi.h"
#include "../cc120x_def.h"

static const registerSetting_t preferredSettings[]= 
{
  {CC120X_IOCFG0,            0x06},
  {CC120X_IOCFG2,            0x58},
  {CC120X_IOCFG3,            0x59},

  {CC120X_SYNC_CFG1,         0xA8},  //8    Sync Word Detection Configuration Reg. 1
  {CC120X_SYNC_CFG0,         0x23},  //9    Sync Word Detection Configuration Reg. 0
  {CC120X_DEVIATION_M,       0x47},  //10    Frequency Deviation Configuration
  {CC120X_MODCFG_DEV_E,      0x0C},  //11    Modulation Format and Frequency Deviation Configur..
  {CC120X_DCFILT_CFG,        0x4B},  //12    Digital DC Removal Configuration
  {CC120X_PREAMBLE_CFG0,     0x8A},  //14    Preamble Detection Configuration Reg. 0
  {CC120X_IQIC,              0xD8},  //15    Digital Image Channel Compensation Configuration
  {CC120X_CHAN_BW,           0x08},  //16    Channel Filter Configuration
  {CC120X_MDMCFG1,           0x42},  //17    General Modem Parameter Configuration Reg. 1
  {CC120X_MDMCFG0,           0x05},  //18    General Modem Parameter Configuration Reg. 0
  {CC120X_SYMBOL_RATE2,      0xA4},  //19    Symbol Rate Configuration Exponent and Mantissa [1..
  {CC120X_SYMBOL_RATE1,      0x7A},  //20    Symbol Rate Configuration Mantissa [15:8]
  {CC120X_SYMBOL_RATE0,      0xE1},  //21    Symbol Rate Configuration Mantissa [7:0]
  {CC120X_AGC_REF,           0x2A},  //22    AGC Reference Level Configuration
  {CC120X_AGC_CS_THR,        0xF6},  //23    Carrier Sense Threshold Configuration
  {CC120X_AGC_CFG1,          0x12},  //27    Automatic Gain Control Configuration Reg. 1
  {CC120X_AGC_CFG0,          0x80},  //28    Automatic Gain Control Configuration Reg. 0
  {CC120X_FIFO_CFG,          0x00},  //29    FIFO Configuration
  {CC120X_FS_CFG,            0x12},  //32    Frequency Synthesizer Configuration
  {CC120X_PKT_CFG2,          0x00},  //38    Packet Configuration Reg. 2
  {CC120X_PKT_CFG0,          0x20},  //40    Packet Configuration Reg. 0
  {CC120X_PA_CFG1,           0x77},  //43    Power Amplifier Configuration Reg. 1
  {CC120X_PKT_LEN,           0xFF},  //46    Packet Length Configuration
  {CC120X_IF_MIX_CFG,        0x1C},  //12032    IF Mix Configuration
  {CC120X_TOC_CFG,           0x03},  //12034    Timing Offset Correction Configuration
  {CC120X_MDMCFG2,           0x02},  //12037    General Modem Parameter Configuration Reg. 2
  {CC120X_FREQ2,             0x56},  //12044    Frequency Configuration [23:16]
  {CC120X_FREQ1,             0xCC},  //12045    Frequency Configuration [15:8]
  {CC120X_FREQ0,             0xCC},  //12046    Frequency Configuration [7:0]
  {CC120X_IF_ADC1,           0xEE},  //12048    Analog to Digital Converter Configuration Reg. 1
  {CC120X_IF_ADC0,           0x10},  //12049    Analog to Digital Converter Configuration Reg. 0
  {CC120X_FS_DIG1,           0x04},  //12050    Frequency Synthesizer Digital Reg. 1
  {CC120X_FS_DIG0,           0x50},  //12051    Frequency Synthesizer Digital Reg. 0
  {CC120X_FS_CAL1,           0x40},  //12054    Frequency Synthesizer Calibration Reg. 1
  {CC120X_FS_CAL0,           0x0E},  //12055    Frequency Synthesizer Calibration Reg. 0
  {CC120X_FS_DIVTWO,         0x03},  //12057    Frequency Synthesizer Divide by 2
  {CC120X_FS_DSM0,           0x33},  //12059    FS Digital Synthesizer Module Configuration Reg. 0
  {CC120X_FS_DVC1,           0xF7},  //12060    Frequency Synthesizer Divider Chain Configuration ..
  {CC120X_FS_DVC0,           0x0F},  //12061    Frequency Synthesizer Divider Chain Configuration ..
  {CC120X_FS_PFD,            0x00},  //12063    Frequency Synthesizer Phase Frequency Detector Con..
  {CC120X_FS_PRE,            0x6E},  //12064    Frequency Synthesizer Prescaler Configuration
  {CC120X_FS_REG_DIV_CML,    0x1C},  //12065    Frequency Synthesizer Divider Regulator Configurat..
  {CC120X_FS_SPARE,          0xAC},  //12066    Frequency Synthesizer Spare
  {CC120X_FS_VCO0,           0xB5},  //12071    FS Voltage Controlled Oscillator Configuration Reg..
  {CC120X_IFAMP,             0x09},  //12079    Intermediate Frequency Amplifier Configuration
  {CC120X_XOSC5,             0x0E},  //12082    Crystal Oscillator Configuration Reg. 5
  {CC120X_XOSC1,             0x03},  //12086    Crystal Oscillator Configuration Reg. 1
};
