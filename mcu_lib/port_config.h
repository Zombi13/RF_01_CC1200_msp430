/*
 * port_config.h
 *
 *  Created on: 3 окт. 2017 г.
 *      Author: Zombi_13
 */

#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_

#include <stdint.h>

void port_init ();

#define Rf_HGM_port_OUT             P2OUT
#define RF_HGM_port_DIR             P2DIR
#define RF_HGM_PIN                  BIT0

#define Rf_RST_port_sel             P2SEL
#define Rf_RST_port_OUT             P2OUT
#define RF_RST_port_DIR             P2DIR
#define RF_RST_PIN                  BIT0

#define Rf_PA_EN_port_OUT           P2OUT
#define RF_PA_EN_port_DIR           P2DIR
#define RF_PA_EN_PIN                BIT0

#define Rf_LNA_EN_port_OUT          P2OUT
#define RF_LNA_EN_port_DIR          P2DIR
#define RF_LNA_EN_PIN               BIT0

/*
#define RF_RESET_N_PORT_OUT  P1OUT
#define RF_RESET_N_PORT_SEL  P1SEL
#define RF_RESET_N_PORT_DIR  P1DIR
#define RF_RESET_N_PIN       BIT4
*/

/* Optional button interrupt configuration *//*
#define     PORTx_VECTOR           PORT2_VECTOR
#define     BUTTON_OUT             P2OUT
#define     BUTTON_DIR             P2DIR
#define     BUTTON_IN              P2IN
#define     BUTTON_SEL             P2SEL
#define     BUTTON_PxIES           P2IES
#define     BUTTON_PxIFG           P2IFG
#define     BUTTON_PxIE            P2IE
#define     BUTTON_REN             P2REN
#define     BUTTON_PIN             BIT2
*/
/****************************************************************
 * PACKET_SEND_RESIVE_PIN
 *
 * for cc1120 BusterPac P2.3
 */

#define PORTx_VECTOR                PORT2_VECTOR
#define PACKET_SEND_RESIVE_OUT      P2OUT
#define PACKET_SEND_RESIVE_DIR      P2DIR
#define PACKET_SEND_RESIVE_IES      P2IES
#define PACKET_SEND_RESIVE_IFG      P2IFG
#define PACKET_SEND_RESIVE_PxIE     P2IE
#define PACKET_SEND_RESIVE_PIN1     BIT5
#define PACKET_SEND_RESIVE_PIN2     BIT4


/****************************************************************
 * Transceiver chip select signal
 *
 * cc1120 BusterPac - CS -P2.7
 */
#define     TRXEM_SPI_CS_1_PORT_SEL     P2SEL
#define     TRXEM_SPI_CS_1_PORT_DIR     P2DIR
#define     TRXEM_SPI_CS_1_PORT_OUT     P2OUT
#define     TRXEM_SPI_CS_1_PIN          BIT7

#define     TRXEM_SPI_CS_2_PORT_SEL     P2SEL
#define     TRXEM_SPI_CS_2_PORT_DIR     P2DIR
#define     TRXEM_SPI_CS_2_PORT_OUT     P2OUT
#define     TRXEM_SPI_CS_2_PIN          BIT6

#define     TRXEM_SPI_CS_N_PORT_SEL     P2SEL
#define     TRXEM_SPI_CS_N_PORT_DIR     P2DIR
#define     TRXEM_SPI_CS_N_PORT_OUT     P2OUT
#define     TRXEM_SPI_CS_N_PIN          BIT7
//uint8_t     TRXEM_SPI_CS_N_PIN;


#define     Entry_comand_mod            0x4F


#endif /* PORT_CONFIG_H_ */

