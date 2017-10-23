//*****************************************************************************
//! @file trx_rf_spi.h  
//    
//! @brief  Common header file for spi access to the different tranceiver 
//          radios. Supports CC1101/CC112X radios
//
//  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************/

#ifndef USCI_B_SPI_H
#define USCI_B_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 */
#include <msp430g2553.h>
#include <stdint.h>
#include "../hal/hal_types.h"
#include "../hal/hal_defs.h"
#include "port_config.h"

/******************************************************************************
 * CONSTANTS
 */ 
 
#define TRXEM_SPI_PORT_SEL       P1SEL
#define TRXEM_SPI_PORT_SEL2      P1SEL2
#define TRXEM_SPI_PORT_OUT       P1OUT
#define TRXEM_SPI_PORT_DIR       P1DIR
#define TRXEM_SPI_PORT_IN        P1IN
                             
#define TRXEM_SPI_MOSI_PIN   BIT7
#define TRXEM_SPI_MISO_PIN   BIT6
#define TRXEM_SPI_SCLK_PIN   BIT5

/* Transceiver chip select signal */ /* Move to port_config.h
#define     TRXEM_SPI_CS_N_PORT_SEL     P1SEL
#define     TRXEM_SPI_CS_N_PORT_DIR     P1DIR
#define     TRXEM_SPI_CS_N_PORT_OUT     P1OUT
#define     TRXEM_SPI_CS_N_PIN          BIT4
*/

#define RADIO_BURST_ACCESS   0x40  
#define RADIO_SINGLE_ACCESS  0x00  
#define RADIO_READ_ACCESS    0x80 
#define RADIO_WRITE_ACCESS   0x00 


/* Bit fields in the chip status byte */
#define STATUS_CHIP_RDYn_BM             0x80
#define STATUS_STATE_BM                 0x70
#define STATUS_FIFO_BYTES_AVAILABLE_BM  0x0F


/******************************************************************************
 * MACROS
 */ 
 
/* Macros for Tranceivers(TRX) */
#define TRXEM_SPI_BEGIN()              st( TRXEM_SPI_CS_N_PORT_OUT &= ~TRXEM_SPI_CS_N_PIN; )
#define TRXEM_SPI_TX(x)                st( IFG2 &= ~UCB0RXIFG; UCB0TXBUF= (x); )
#define TRXEM_SPI_WAIT_DONE()          st( while(!(IFG2 & UCB0RXIFG)); )
#define TRXEM_SPI_RX()                 UCB0RXBUF
#define TRXEM_SPI_WAIT_MISO_LOW(x)     st( uint8_t count = 200; \
                                           while(TRXEM_SPI_PORT_IN & TRXEM_SPI_MISO_PIN) \
                                           { \
                                              __delay_cycles(5000); \
                                              count--; \
                                              if (count == 0) break; \
                                           } \
                                           if(count>0) (x) = 1; \
                                           else (x) = 0; )

#define TRXEM_SPI_END()                st(  TRXEM_SPI_CS_N_PORT_OUT |= TRXEM_SPI_CS_N_PIN; )

/******************************************************************************
 * TYPEDEFS
 */ 
 
typedef struct
{
  uint16_t  addr;
  uint8_t   data;
}registerSetting_t;

typedef uint8_t rfStatus_t;

/******************************************************************************
 * PROTOTYPES
 */

void trxRfSpiInterfaceInit(uint8_t prescalerValue);
rfStatus_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16 len);
rfStatus_t trxSpiCmdStrobe(uint8_t cmd);

/* CC112X specific prototype function */
rfStatus_t trx16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len);

#ifdef  __cplusplus
}
#endif


#endif //TRX_RF_SPI_H
