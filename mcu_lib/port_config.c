/*
 * port_config.c
 *
 *  Created on: 6 окт. 2017 г.
 *      Author: Zombi_13
 */
#include <msp430g2553.h>
#include "port_config.h"

void port_init (){
    // select chip select bit TRXEM_SPI_CS_N_PORT_SEL as a port //CS_1
    TRXEM_SPI_CS_1_PORT_SEL &= ~TRXEM_SPI_CS_1_PIN;
    TRXEM_SPI_CS_1_PORT_DIR |=  TRXEM_SPI_CS_1_PIN;
    TRXEM_SPI_CS_1_PORT_OUT |=  TRXEM_SPI_CS_1_PIN;
    // select chip select bit TRXEM_SPI_CS_N_PORT_SEL as a port //CS_2
    TRXEM_SPI_CS_2_PORT_SEL &= ~TRXEM_SPI_CS_2_PIN;
    TRXEM_SPI_CS_2_PORT_DIR |=  TRXEM_SPI_CS_2_PIN;
    TRXEM_SPI_CS_2_PORT_OUT |=  TRXEM_SPI_CS_2_PIN;

    //TRXEM_SPI_CS_N_PIN = TRXEM_SPI_CS_1_PIN;

    RF_HGM_port_DIR  |= RF_HGM_PIN ;
    Rf_HGM_port_OUT  |= RF_HGM_PIN ;

    RF_RST_port_DIR  |= RF_RST_PIN ;
    Rf_RST_port_OUT  |= RF_RST_PIN ;

    RF_PA_EN_port_DIR  |= RF_PA_EN_PIN ;
    Rf_PA_EN_port_OUT  &= ~ RF_PA_EN_PIN ;

    RF_LNA_EN_port_DIR |= RF_LNA_EN_PIN ;
    Rf_LNA_EN_port_OUT |= RF_LNA_EN_PIN ;

    PACKET_SEND_RESIVE_DIR &= ~(PACKET_SEND_RESIVE_PIN1 + PACKET_SEND_RESIVE_PIN2);
    PACKET_SEND_RESIVE_OUT &= ~(PACKET_SEND_RESIVE_PIN1 + PACKET_SEND_RESIVE_PIN2);

//    PACKET_SEND_RESIVE_IES |=  PACKET_SEND_RESIVE_PIN1 + PACKET_SEND_RESIVE_PIN2;                         // Hi/lo edge
    PACKET_SEND_RESIVE_PxIE |= PACKET_SEND_RESIVE_PIN1 + PACKET_SEND_RESIVE_PIN2;
    PACKET_SEND_RESIVE_IFG &= ~(PACKET_SEND_RESIVE_PIN1 + PACKET_SEND_RESIVE_PIN2);                           // IFG cleared



}



