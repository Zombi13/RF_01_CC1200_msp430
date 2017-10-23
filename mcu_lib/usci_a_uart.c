/*
 * USCI_A0_UART.c
 *
 *  Created on: Nov 21, 2016
 *      Author: Z13
 */

#include "usci_a_uart.h"

#include <msp430.h>
#include "port_config.h"

extern int worck_mod;


char string1[6];
char i=0;
char j = 0;
int caunt = 0;
unsigned char uart_rx=5;
unsigned char uart_resiv_isr = 0;
//-----------------------------------Usart_init-------------------------------------
void UART_init(void){


    UART_PORT_SEL    |= UART_PIN_RXD + UART_PIN_TXD;
    UART_PORT_SEL2   |= UART_PIN_RXD + UART_PIN_TXD;
    UART_PORT_DIR    |= UART_PIN_TXD;
    UART_PORT_DIR    &= ~UART_PIN_RXD;

	UCA0CTL1 |= UCSWRST;                     	// Resrt USCI
	//Контроль чётности отключен, Младший бит первый, 8-битные данные,
	//Один стоп-бит, Режим UART, Асинхронный режим;
	UCA0CTL0 =UCMODE_0;
	UCA0CTL1 |= UCSSEL_2 +UCSWRST;				//источник тактового сигнала - SMCLK
	UCA0BR0 = 104;								// 1MHz 9600
	UCA0BR1 = 0;								// 1MHz 9600
	UCA0MCTL = UCBRS1 + UCBRS0;               	// Modulation UCBRSx = 3
	//UCA0MCTL = UCBRS0;                          // Modulation UCBRSx = 1


	UCA0CTL1 &= ~UCSWRST;                     	// **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          	// Enable USCI_A0 RX interrupt


}

void UART_send(char data){
	while (!(IFG2&UCA0TXIFG));                 		// USCI_A0 TX buffer ready?
	UCA0TXBUF = data;
}

void UART_sbuf(unsigned char num,unsigned char *buf)
{int i=0;

while(i<num){  UART_send(*buf);
 //              WDTCTL =WDTPW+WDTCNTCL;
             i++;
             buf++;
               }

 }
void UART_Send_String(char *data){
    while (data !=0){
        while (!(IFG2&UCA0TXIFG));                      // USCI_A0 TX buffer ready?
        UCA0TXBUF = *data;
        data++;
    }
}

void UART_Write_String(char* string)
{
    if(*string == 0)
    {
    while (!(IFG2&UCA0TXIFG)); // Doi gui xong ky tu truoc
    UCA0TXBUF= '0'; // Moi cho phep gui ky tu tiep theo
    }
    while(*string) // Het chuoi ky tu thi thoat
        {
        while (!(IFG2&UCA0TXIFG)); // Doi gui xong ky tu truoc
        UCA0TXBUF= *string; // Moi cho phep gui ky tu tiep theo
        string ++; // Lay ky tu tiep theo

        }
    UART_send('\n');
    UART_send('\r');
}

//---------------------------------/init---------------------------------------------
/*

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  UCA0TXBUF = string1[i++];                 // TX next character

  if (i == sizeof string1)                  // TX over?
    IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt

}

*/
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uart_rx = UCA0RXBUF;
    if (uart_rx == Entry_comand_mod){ //Entry_comand_mod
        worck_mod = 0;
        UART_send(worck_mod);
    }else
    uart_resiv_isr = 1;
}





