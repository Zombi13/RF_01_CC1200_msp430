/*
 * USA0_UART.h
 *
 *  Created on: Nov 21, 2016
 *      Author: Z13
 */

#ifndef USCI_A_UART_H_
#define USCI_A_UART_H_

/******************************************************************************
 * CONSTANTS
 */

#define UART_PORT_OUT      P1OUT
#define UART_PORT_SEL      P1SEL
#define UART_PORT_SEL2     P1SEL2
#define UART_PORT_DIR      P1DIR
#define UART_PORT_REN      P1REN
#define UART_PIN_TXD       BIT2
#define UART_PIN_RXD       BIT1

//размер буфера
#define Buff_Size 128


unsigned int UARTRxCount;

/******************************************************************************
 *
 */

void UART_init(void);
void UART_send(char data);
void UART_sbuf(unsigned char num,unsigned char *buf);
void UART_Send_String(char *data);
void UART_Write_String(char* string);

void UART_TX_buff_add(unsigned char data);
void UART_TX_buff_get();

void UART_RX_Buff_add(unsigned char data);
unsigned char UART_RX_Buff_get();


#endif /* USA0_UART_H_ */
