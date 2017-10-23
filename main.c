//*****************************************************************************
//! @file        cc120x_easy_link_rx.c
//
//! @brief     This program sets up an easy link between two trxEB's with
//                CC120x EM's connected. 
//                The program can take any recomended register settings exported
//                from SmartRF Studio 7 without any modification with exeption 
//                from the assumtions decribed below.
//  
//                The following asumptions must be fulfilled for the program
//                to work:
//                
//                1. GPIO2 has to be set up with GPIO2_CFG = 0x06
//                   PKT_SYNC_RXTX for correct interupt
//                2. Packet engine has to be set up with status bytes enabled 
//                   PKT_CFG1.APPEND_STATUS = 1
//
//  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/

//****************************************************************************/


/*****************************************************************************
* INCLUDES
*/
#include "stdio.h"
#include "msp430g2553.h"
#include <stdint.h>

//#include "trx_rf_int.h"
#include "rf_moduls/cc120x_spi.h"
#include "mcu_lib/usci_a_uart.h"
#include "mcu_lib/usci_b_spi.h"
#include "mcu_lib/port_config.h"
#include "rf_moduls/cc1200/cc120x_def.h"


//#include "config_1101\cc1101_def.h"

//#include "rf_conf\CC1200_2-FSK_1.2kbps_reg_config.c"
#include "rf_moduls/cc1200/rf_conf\CC1200_2-GFSK_38.4k_reg_config.c"
//#include "rf_conf\CC1200_2-GFSK_ETSI_100k_reg_config.c"
//#include "rf_conf\CC1200_2-GFSK_IEEE802.15.4g_reg_config.c"
//#include "rf_conf\CC1200_4-fsk_reg_config.c"
//#include "rf_conf\CC1200_4-GFSK_130k_reg_config.c"
//#include "rf_conf\CC1200_4-GFSK_max_config.c"
//#include "rf_conf\CC1200_OOK_4.8k_reg_config.c"
//#include "rf_conf\CC1200_2-fsk_500k_reg_config.c"
//#include "rf_conf\CC1200_2-fsk_EasyMod_reg_config.c"
//#include "rf_conf\CC1200_2-fsk_1.2_reg_config.c"
//#include "rf_conf\CC1200_2-fsk_t01_reg_config.c"
//#include "rf_conf\CC1200_2-fsk_t02_config.c"
//#include "rf_conf\CC1200_2-fsk_t03_EM_config.c"
//#include "rf_conf\CC1200_4-fsk_t04_EM_config.c"
//#include "rf_conf\CC1200_ook__t05_EM_reg_config.c"

//#include "rf_conf\CC1200_920_2GFSK_38_reg_config.h"
//#include "rf_conf\CC1200_920_2GFSK_50_reg_config.h"
//#include "rf_conf\CC1200_920_1000_4GFSK_reg_config.h"
//#include "rf_moduls/cc1200/rf_conf/CC1200_920_100_2gfsk_reg_config.h"
//#include "rf_conf\CC1200_920_38_2g_reg_config.h"

//#include "config_1101\CC1101_simple_link_reg_config.c"
//#include "config_1101\CC1101_simple_link_reg_config1.c"
//#include "config_1101\CC1101_simple_link_reg_config2.c"
//#include "config_1101\CC1101_gfsk_4.8k_reg_config.c"

//#include "rf_conf\CC1200_2-gfsk_50kbps_reg_config.c"

//#include "config_1101\CC1101_simple_link_reg_config1.h"
//#include "rf_conf\CC1200_simple_link_reg_config.h"

//#include "config_1120\CC1120_920_2GFSK_50_reg_config.h"
//#include "config_1120\CC1120_920_38_2g_reg_config.h"
/******************************************************************************
* DEFINES
*/
#define ISR_ACTION_REQUIRED 1
#define ISR_IDLE            0

#define RX_FIFO_ERROR       0x11
#define TRX_int             0x06
//cc1101
#define LNA_EN              0x5C
#define PA_EN               0x5B
//cc1200
//#define LNA_EN              0x58
//#define PA_EN               0x59

#define PKT_LEN                      2       // Packet length has to be within fifo limits ( 1 - 127 bytes)

//#define Entry_comand_mod            0x4F    Move to port_config.h
#define Extended_Register_Space     0x2F    // Extended Register Space Access (0x2F)

#define RF_Set_mod                  0x00
#define RF_TX_mod                   0x01
#define RF_RX_mod                   0x02
#define RF_Macros_mod               0x03
#define RF_Strobe_mod               0x04
#define RF_Read_reg_mod             0x05
#define RF_Write_reg_mod            0x06

#define RF_LNA_pow_mod              0x09
#define RF_PA_pow_mod               0x0A

/******************************************************************************
* STATIC FUNCTIONS
*/
void registerConfig(void);
//void cc120xRunRX(void);
void createPacket(uint8_t randBuffer[]);
void RF_TX (uint8_t *pData, uint8_t len);
void RF_RX (uint8_t *rxBuffer);
void RX_Data_Processing ( uint8_t *rxBuffer, uint8_t rx_Bytes);

/******************************************************************************
* LOCAL VARIABLES
*/
volatile uint8_t      packetSemaphore;
volatile uint8_t      packetSemaphore1;
volatile uint8_t      paket_resiv_isr;
volatile uint8_t      worck_mod;
volatile uint8_t      RF_comand_reciv = 0;

static uint32       packetCounter = 0;


uint8_t             UTXbuf[8]={0x1F,2,1,0,0,0,0,0};

uint8_t             rx_test[256];
uint8_t             adr_test=0x00;
uint8_t             rx_stete[64];
uint8_t             rx_stete1[8];
uint8_t             Write_sw = 0;
uint8_t             RS=0,RS1=0;
uint8_t             temp[13]={'0'};


/******************************************************************************
 * @fn rx_command_paket
 *
 * rx_command_paket[0]          - transmit packet length. set by the transmitter
 * rx_command_paket[1]          - Entry_comand_mod
 * rx_command_paket[2]          - worck_mod
 * rx_command_paket[3]          - Extended Register Space Access (0x2F)
 * rx_command_paket[4]          - Register Addres / Macros num
 * rx_command_paket[5]          - Register Value for Write mod
 * rx_command_paket[6 to 128]   - need to packet remapping. See RF_RX -> receive commands mod
 *
 */
uint8_t rx_command_paket[128] = {0};
uint8_t rx_command_paket1[12] = {0};

/******************************************************************************
 * @fn tx_command_paket
 *
 * rx_command_paket[0]          - transmit packet length. set by the transmitter
 * rx_command_paket[1]          - Entry_comand_mod
 * rx_command_paket[2]          - worck_mod
 * rx_command_paket[3]          - Extended Register Space Access (0x2F)
 * rx_command_paket[4]          - Register Addres / Macros num
 * rx_command_paket[5]          - Register Value for Write mod
 * rx_command_paket[6 to 128]   - need to packet remapping. See RF_RX -> receive commands mod
 *
 */
uint8_t tx_command_paket[12] = {0};

/******************************************************************************
* extern VARIABLES
*/
extern unsigned char uart_rx;
extern unsigned char uart_resiv_isr;

/******************************************************************************
 * main.c
 */
int main(void) {

    uint8_t rxBuffer[128] = {0};
    uint8_t rx_Bytes;
    uint8_t marcStatus;
    uint8_t rf_read;
    uint8_t rf_write;
    worck_mod =0;
    // Initialize packet buffer of size PKT_LEN + 1
    uint8_t txBuffer[PKT_LEN+1] = {0};


    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    if (CALBC1_1MHZ==0xFF)						// If calibration constant erased
      {
        while(1);                               	// do not load, trap CPU!!
      }
      DCOCTL = 0;                               	// Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                    	// Set DCO
      DCOCTL = CALDCO_1MHZ;


    trxRfSpiInterfaceInit(2);
    UART_init();
    port_init ();

    // Write radio registers
    registerConfig();

    __delay_cycles(100);    //Important!!! need to reconfigure port

    // test Write registers
    //rx_stete1[0] = trx8BitRegAccess((RADIO_BURST_ACCESS|RADIO_READ_ACCESS),adr_test,rx_test,20);


    __bis_SR_register( GIE ); // Enable global interrupts

    //Start MOD
    uart_resiv_isr = ISR_ACTION_REQUIRED;
    worck_mod = RF_Set_mod; // RF_TX_mod // RF_RX_mod // RF_Set_mod

    // Loop until left button is pushed (exits application)
    while(TRUE){

        switch (worck_mod){
        case RF_Set_mod:{

            if (uart_rx == Entry_comand_mod){                       //Entry_comand_mod detected
                rx_command_paket[1] = Entry_comand_mod;
            }

            if (uart_resiv_isr == ISR_ACTION_REQUIRED){             //UART commands mod
                uart_resiv_isr = ISR_IDLE;

                if (rx_command_paket[1] == Entry_comand_mod){       //if Entry_comand_mod detected
                    rx_command_paket[2] = uart_rx;                  //change Worck_mod
                    rx_command_paket[1] = 0x00;                     // clear Entry_comand_mod

                    if (rx_command_paket[2] < 0x03 || rx_command_paket[2] == 0x08
                            || rx_command_paket[2] == 0x11 || rx_command_paket[2] == 0x12){
                        worck_mod = rx_command_paket[2];            // switch to RX/TX mod
                        UART_send(worck_mod);
                    }
                }else{
                    if (uart_rx == Extended_Register_Space)         //Extended_Register_Space
                        rx_command_paket[3] = uart_rx;
                    else{
                        rx_command_paket[3] = 0x00;                 // Reset Extended_Register_Space

                        if (rx_command_paket[2] == RF_Macros_mod || rx_command_paket[2] == RF_Strobe_mod
                                || rx_command_paket[2] == RF_Read_reg_mod){   // switch to 3 - 5 mod
                            rx_command_paket[4] = uart_rx;
                            worck_mod = rx_command_paket[2];
                            UART_send(worck_mod);
                        }

                        if (rx_command_paket[2] == RF_Write_reg_mod ){   // switch to 6 mod
                            if(Write_sw ==0){
                                rx_command_paket[4] = uart_rx;
                                Write_sw = 1;
                            }else{
                                rx_command_paket[5] = uart_rx;
                                Write_sw = 0;
                                worck_mod = rx_command_paket[2];
                                UART_send(worck_mod);
                            }

                        }
                        if (rx_command_paket[2] == 0x07 ){
                            worck_mod = rx_command_paket[2];
                            packetSemaphore = ISR_ACTION_REQUIRED;
                        }
                        if (rx_command_paket[2] == RF_LNA_pow_mod || rx_command_paket[2] == RF_PA_pow_mod){   // switch to 3 - 5 mod
                            rx_command_paket[5] = uart_rx;
                            worck_mod = rx_command_paket[2];
                            UART_send(worck_mod);
                        }
                        if (rx_command_paket[2] >= 0x11 ){          // error switch to 0 mod
                            worck_mod = 0x00;
                            UART_send(0xFF);
                            UART_send(worck_mod);
                            UART_send(0xFF);

                        }
                        paket_resiv_isr = ISR_ACTION_REQUIRED;
                    }
                }
            }
/*
            switch (pkt_poz){
            case 0:{

            }
            }
*/
            if (RF_comand_reciv == ISR_ACTION_REQUIRED){    // RF receive commands mod
                for (int k=0; k <= rx_Bytes; k++){
                    rx_command_paket[k] = rxBuffer[k];
                }
                worck_mod = rx_command_paket[2];
                UART_send( worck_mod);
                RF_comand_reciv ++;

            } else if (RF_comand_reciv == 0x02){
                RF_comand_reciv = ISR_IDLE;
                worck_mod = RF_RX_mod;
            }

            if (worck_mod == RF_TX_mod || worck_mod ==0x07 || worck_mod == 0x08 || worck_mod == 0x0b){ //RF_TX

/*For CC1101
                rf_write = TRX_int;
                cc120xSpiWriteReg( IOCFG2, &rf_write, 1);    //GDO2 - int
                rf_write = PA_EN;
                cc120xSpiWriteReg( IOCFG0, &rf_write, 1);      //GDO0 - PA_EN
*/
                packetSemaphore = ISR_ACTION_REQUIRED;

                __delay_cycles(100);    //Important!!! need to reconfigure port

            }
            if (worck_mod == RF_RX_mod || worck_mod == 0x0C){ //RF_RX

/* For CC1101
                rf_write = LNA_EN;
                cc120xSpiWriteReg( IOCFG2, &rf_write, 1);     //GDO2 - LNA_EN
                rf_write = TRX_int;
                cc120xSpiWriteReg( IOCFG0, &rf_write, 1);    //GDO0 - int
*/
                __delay_cycles(100);        //Important!!! need to reconfigure port

                // Set radio back in RX
                trxSpiCmdStrobe(CC120X_SRX);

            }

            break;
        }

        case RF_TX_mod:{    //RF_TX_mode
            // Wait for packet received interrupt
            if(packetSemaphore == ISR_ACTION_REQUIRED) {
                // Reset packet semaphore
                packetSemaphore = ISR_IDLE;

                // Create a random packet with PKTLEN + 2 byte packet counter + n x random bytes
                createPacket(txBuffer);

                // Flush TX Fifo
                trxSpiCmdStrobe(CC120X_SFTX);

                // Write packet to tx fifo
                cc120xSpiWriteTxFifo(txBuffer,sizeof(txBuffer));

                // Strobe TX to send packet
                trxSpiCmdStrobe(CC120X_STX);
                UART_send( 0xF0);
            }
            break;
        }

        case RF_RX_mod:{
            if(packetSemaphore == ISR_ACTION_REQUIRED) {
                // Reset packet semaphore
                packetSemaphore = ISR_IDLE;

                // Read number of bytes in rx fifo
                rx_stete1[2] = cc120xSpiReadReg(CC120X_NUM_RXBYTES, &rx_Bytes, 1);
                //rx_Bytes = 5 ;
                // Check that we have bytes in fifo

                if(rx_Bytes != 0){
                    // Read marcstate to check for RX FIFO error
                    cc120xSpiReadReg(CC120X_MARCSTATE, &marcStatus, 1);

                    // Mask out marcstate bits and check if we have a RX FIFO error
                    if((marcStatus & 0x1F) == RX_FIFO_ERROR){

                        // Flush RX Fifo
                        trxSpiCmdStrobe(CC120X_SFRX);
                        UART_send(0xFF);
                        UART_send(0x01);
                        UART_send(0xFF);

                    }else{
                        // Read n bytes from rx fifo
                        rx_stete1[5] = cc120xSpiReadRxFifo(rxBuffer, rx_Bytes);

                        // Check CRC ok (CRC_OK: bit7 in second status byte)
                        // This assumes status bytes are appended in RX_FIFO
                        // (PKT_CFG1.APPEND_STATUS = 1.)
                        // If CRC is disabled the CRC_OK field will read 1
                        if(rxBuffer[rx_Bytes-1] & 0x80){
                            if (rxBuffer[1] == Entry_comand_mod){      //Entry_comand_mod
                                RF_comand_reciv = ISR_ACTION_REQUIRED;
                                worck_mod = RF_Set_mod;
                                UART_sbuf(rx_Bytes,rxBuffer);
                                break;
                            }

                            // Update packet counter
                            packetCounter++;

                            RS1 = rxBuffer[rx_Bytes-2];
                            if(RS1>128){
                                RS=(RS1-128)-8;
                            }
                            else{
                                RS=(RS1+128)-8;
                            }

                            UTXbuf[0]=0x1F;
                            UTXbuf[1]=3;
                            UTXbuf[2]=0x10;

                            UTXbuf[3]=rxBuffer[2];
                            UTXbuf[4]=RS;
                            //UART_sbuf(5,UTXbuf);
                            for (int u=0;u<5;u++){
                                UART_send(UTXbuf[u]);
                            }

                            UTXbuf[3]=rxBuffer[2] +1;
                            UTXbuf[4]=rxBuffer[rx_Bytes-1];
                            //UART_sbuf(5,UTXbuf);
                            for (int u=0;u<5;u++){
                                UART_send(UTXbuf[u]);
                            }

                           // UART_send(rx_Bytes);
                        }
                        else{
                            UART_send(0xFF);
                            UART_send(0x01);
                            UART_send(0xFF);

                        }

                    }

                }
           // Flush RX Fifo
            trxSpiCmdStrobe(CC120X_SFRX);
            // Set radio back in RX
            trxSpiCmdStrobe(CC120X_SRX);

            }


            break;
        }

        case RF_Macros_mod:{    //RF_macros_mode
            if (paket_resiv_isr){
                paket_resiv_isr = ISR_IDLE;

                worck_mod = RF_Set_mod;

                switch (rx_command_paket[4]) {
                case 0: {
                    cc120xSpiReadReg(CC120X_NUM_RXBYTES, &rx_Bytes, 1);
                    cc120xSpiReadReg(CC120X_NUM_TXBYTES, &rf_read, 1);
                    UART_send(rx_Bytes);
                    UART_send(rf_read);

                    break;
                }
                case 1: {

                    rx_stete1[1] = cc120xSpiReadReg(CC120X_NUM_RXBYTES, &rx_Bytes, 1);
                    rx_stete1[2] = cc120xSpiReadReg(CC120X_NUM_TXBYTES, &rf_read, 1);

                    UART_send(rx_Bytes);
                    UART_send(rx_stete1[1]);

                    UART_send(rf_read);
                    UART_send(rx_stete1[2]);

                    break;
                }
                case 2: {// Read n bytes from rx fifo
                    cc120xSpiReadRxFifo(rxBuffer, 5);
                    UART_sbuf(5,rxBuffer);

                    break;
                }
                case 3: {// Check CRC ok (CRC_OK: bit7 in second status byte)
                    UART_send(rxBuffer[rx_Bytes-1] & 0x80);
                    break;
                }
                case 4: {//MARCSTATE
                    rx_stete1[3] = cc120xSpiReadReg(CC120X_MARCSTATE, &marcStatus, 1);
                    UART_send(marcStatus);

                    break;
                }
                case 5: { // Flush RX Fifo
                    UART_send(trxSpiCmdStrobe(CC120X_SFRX));
                    break;
                }

                case 6: {// Set radio back in RX
                    UART_send(trxSpiCmdStrobe(CC120X_SRX));
                    break;
                }
                case 7: {// Flush RX Fifo and Set radio back in RX
                    UART_send(trxSpiCmdStrobe(CC120X_SFRX));
                    UART_send(trxSpiCmdStrobe(CC120X_SRX));
                    worck_mod = RF_RX_mod;
                    break;
                }
                case 8: {// Flush TX Fifo and Set radio in RF_Set_mod
                    // Flush TX Fifo
                    trxSpiCmdStrobe(CC120X_SFTX);
                    packetSemaphore = 1;
                    worck_mod = RF_Set_mod;
                    break;
                }
                case 9: {
                    //trx8BitRegAccess(0x00,CC120X_BURST_TXFIFO, &rxBuffer, 5);
                    cc120xSpiReadReg(CC120X_BURST_TXFIFO, rxBuffer, 5);
                    UART_sbuf(5,rxBuffer);
                    break;
                 }
                case 10: {// Read n bytes from tx fifo
                    createPacket(txBuffer);

                    // Write packet to tx fifo
                    rx_stete[1] = cc120xSpiWriteTxFifo(txBuffer,sizeof(txBuffer));

                    break;
                 }
                case 11: {// Read n bytes from tx fifo
                    // Strobe TX to send packet
                    rx_stete[2] = trxSpiCmdStrobe(CC120X_STX);
                     break;
                  }

                default: {

                    rx_stete1[3] = cc120xSpiReadReg(CC120X_MARCSTATE, &marcStatus, 1);
                    UART_send(marcStatus);

                    break;
                }

                }
            }
            break;
        }

        case RF_Strobe_mod:{
            if (paket_resiv_isr){
                paket_resiv_isr = ISR_IDLE;

                worck_mod = RF_Set_mod;

                UART_send(trxSpiCmdStrobe(rx_command_paket[4]));

            }

            break;
        }

        case RF_Read_reg_mod:{
            if (paket_resiv_isr){
                paket_resiv_isr = ISR_IDLE;

                worck_mod = RF_Set_mod;

                cc120xSpiReadReg(rx_command_paket[4], &rf_read, 1);
                UART_send(rf_read);

            }

            break;
        }

        case RF_Write_reg_mod:{
            if (paket_resiv_isr){
                paket_resiv_isr = ISR_IDLE;

                worck_mod = RF_Set_mod;

                cc120xSpiWriteReg(rx_command_paket[4], &rx_command_paket[5], 1);

                __delay_cycles(100);        //Important!!! need to reconfigure port

                //proverka
                cc120xSpiReadReg(rx_command_paket[4], &rf_read, 1);
                UART_send(rx_command_paket[4]);
                UART_send(rf_read);

            }
            break;
        }

        case 07:{
            rx_command_paket[0] = 0x05;
            rx_command_paket[1] = Entry_comand_mod;

            if (uart_resiv_isr == ISR_ACTION_REQUIRED){
                uart_resiv_isr = ISR_IDLE;
                if(Write_sw ==0){
                    rx_command_paket[2] = uart_rx;
                    Write_sw++;
                }else if (uart_rx == Extended_Register_Space)         //Extended_Register_Space
                        rx_command_paket[3] = uart_rx;
                    else{
                        if(Write_sw ==1){
                            rx_command_paket[4] = uart_rx;
                            Write_sw ++;
                        }else{
                            rx_command_paket[5] = uart_rx;
                            Write_sw = 0;
                            if(packetSemaphore == ISR_ACTION_REQUIRED) {
                                // Reset packet semaphore
                                packetSemaphore = ISR_IDLE;

                                // Flush TX Fifo
                                trxSpiCmdStrobe(CC120X_SFTX);

                                // Write packet to tx fifo
                                cc120xSpiWriteTxFifo(rx_command_paket,sizeof(rx_command_paket));

                                // Strobe TX to send packet
                                trxSpiCmdStrobe(CC120X_STX);
                                UART_send( 0xF0);
                            }
                        }

                    }
                }
            break;
        }

        case 8:{
            if(packetSemaphore == ISR_ACTION_REQUIRED) {
                // Reset packet semaphore
                packetSemaphore = ISR_IDLE;

                rx_command_paket1[0] = 0x05;
                rx_command_paket1[1] = Entry_comand_mod;
                rx_command_paket1[2] = 0x05;
                rx_command_paket1[3] = 0x00;
                rx_command_paket1[4] = 0x02;
                rx_command_paket1[5] = 0x00;

                // Flush TX Fifo
                trxSpiCmdStrobe(CC120X_SFTX);

                // Write packet to tx fifo
                cc120xSpiWriteTxFifo(rx_command_paket1,sizeof(rx_command_paket1));

                cc120xSpiReadReg(CC120X_NUM_RXBYTES, &rx_Bytes, 1);
                cc120xSpiReadReg(CC120X_NUM_TXBYTES, &rf_read, 1);
                UART_send(rx_Bytes);
                UART_send(rf_read);
                // Strobe TX to send packet
                rx_stete[2] = trxSpiCmdStrobe(CC120X_STX);

                UART_send( rx_stete[2]);
                //UART_send( 0xF0);
//                cc120xSpiReadReg(IOCFG2, &rf_read, 1);
                UART_send(rf_read);

            }
            break;
        }

        case RF_LNA_pow_mod:{
            if (paket_resiv_isr){
                  paket_resiv_isr = ISR_IDLE;

                  worck_mod = RF_Set_mod;

                  rf_write = rx_command_paket[5] | 0xC7;
//cc1101                  cc120xSpiWriteReg(FIFOTHR, &rf_write, 1);
                  __delay_cycles(100);        //Important!!! need to reconfigure port
                  //proverka
//cc1101                   cc120xSpiReadReg(FIFOTHR, &rf_read, 1);
//cc1101                   UART_send(FIFOTHR);
                  UART_send(rf_read);

              }
            break;
        }
        case RF_PA_pow_mod:{
            if (paket_resiv_isr){
                  paket_resiv_isr = ISR_IDLE;

                  worck_mod = RF_Set_mod;

//cc1101          cc120xSpiWriteReg(0x3E, &rx_command_paket[5], 1); //cc1101
                  cc120xSpiWriteReg(0x2B, &rx_command_paket[5], 1); //cc1200/cc1120
                  __delay_cycles(100);        //Important!!! need to reconfigure port
                  //proverka
                  cc120xSpiReadReg(0x3E, &rf_read, 1);
                  UART_send(0x3E);
                  UART_send(rf_read);

              }
            break;
        }
        case 0x11:{
            // Wait for packet received interrupt
             if(packetSemaphore == ISR_ACTION_REQUIRED) {
                 // Reset packet semaphore
                 packetSemaphore = ISR_IDLE;

                // Create a random packet with PKTLEN + 2 byte packet counter + n x random bytes
                createPacket(txBuffer);

                RF_TX (txBuffer,sizeof(txBuffer));
                UART_send( 0xF0);

              }
            break;
        }

        case 0x12:{
            if(packetSemaphore == ISR_ACTION_REQUIRED) {
                // Reset packet semaphore
                packetSemaphore = ISR_IDLE;

                RF_RX (rxBuffer);

                // Check CRC ok (CRC_OK: bit7 in second status byte)
                // This assumes status bytes are appended in RX_FIFO
                // (PKT_CFG1.APPEND_STATUS = 1.)
                // If CRC is disabled the CRC_OK field will read 1
                if(rxBuffer[rx_Bytes-1] & 0x80){
                    if (rxBuffer[1] == Entry_comand_mod){      //Entry_comand_mod
                        RF_comand_reciv = ISR_ACTION_REQUIRED;
                        worck_mod = RF_Set_mod;
                        UART_sbuf(rx_Bytes,rxBuffer);
                        break;
                    }

                    RX_Data_Processing (rxBuffer, rx_Bytes);


                }
                else{
                    UART_send(0xFF);
                    UART_send(0x01);
                    UART_send(0xFF);
                }

           // Flush RX Fifo
            trxSpiCmdStrobe(CC120X_SFRX);
            // Set radio back in RX
            trxSpiCmdStrobe(CC120X_SRX);

            }


            break;
        }
        }



    }


}

/*******************************************************************************
 *
 * @brief       input parameters
 *
 * @param       *pData - pointer to data array that is written to TX FIFO
 * @param       len    - Length of data array to be written
 *
 */
void RF_TX (uint8_t *pData, uint8_t len){

    // Flush TX Fifo
    trxSpiCmdStrobe(CC120X_SFTX);

    // Write packet to tx fifo
    cc120xSpiWriteTxFifo(pData,len);

    // Strobe TX to send packet
    trxSpiCmdStrobe(CC120X_STX);
}

/*******************************************************************************
 *
 * @brief        input parameters
 *
 * @param       *pData - pointer to data array where RX FIFO bytes are saved
 *
 */
void RF_RX (uint8_t *rxBuffer){
    uint8_t rx_Bytes;
    uint8_t marcStatus;
    // Read number of bytes in rx fifo
    rx_stete1[2] = cc120xSpiReadReg(CC120X_NUM_RXBYTES, &rx_Bytes, 1);
    //rx_Bytes = 5 ;
    // Check that we have bytes in fifo

    if(rx_Bytes != 0){
        // Read marcstate to check for RX FIFO error
        cc120xSpiReadReg(CC120X_MARCSTATE, &marcStatus, 1);

        // Mask out marcstate bits and check if we have a RX FIFO error
        if((marcStatus & 0x1F) == RX_FIFO_ERROR){

            // Flush RX Fifo
            trxSpiCmdStrobe(CC120X_SFRX);
            // Error message
            UART_send(0xFF);
            UART_send(0x01);
            UART_send(0xFF);

        }else{
            // Read n bytes from rx fifo
            rx_stete1[5] = cc120xSpiReadRxFifo(rxBuffer, rx_Bytes);


        }
    }
}

/*******************************************************************************
 * @fn          registerConfig
 *
 * @brief
 *
 * @param       *rxBuffer
 * @param       rx_Bytes
 *
 * @return      none
 */
void RX_Data_Processing ( uint8_t *rxBuffer, uint8_t rx_Bytes) { //uint8_t *rxBuffer, uint8_t rx_Bytes

    RS1 = rxBuffer[rx_Bytes-2];

    if(RS1>128){
        RS=(RS1-128)-8;
    }
    else{
        RS=(RS1+128)-8;
    }

    UTXbuf[0]=0x1F;
    UTXbuf[1]=3;
    UTXbuf[2]=0x10;

    UTXbuf[3]=rxBuffer[2];
    UTXbuf[4]=RS;
    //UART_sbuf(5,UTXbuf);
    for (int u=0;u<5;u++){
        UART_send(UTXbuf[u]);
    }

    UTXbuf[3]=rxBuffer[2] +1;
    UTXbuf[4]=rxBuffer[rx_Bytes-1];
    //UART_sbuf(5,UTXbuf);
    for (int u=0;u<5;u++){
        UART_send(UTXbuf[u]);
    }
}

/*******************************************************************************
* @fn          registerConfig
*
* @brief       Write register settings as given by SmartRF Studio found in
*              cc120x_easy_link_reg_config.h
*
* @param       none
*
* @return      none
*/
void registerConfig(void){
  
  uint8_t writeByte;
  
  // Reset radio
  trxSpiCmdStrobe(CC120X_SRES);
  
  // Write registers to radio
  uint16 i;
  for( i = 0; i < (sizeof  preferredSettings/sizeof(registerSetting_t)); i++) {
    writeByte =  preferredSettings[i].data;
    cc120xSpiWriteReg( preferredSettings[i].addr, &writeByte, 1);
  }
}


/******************************************************************************
 * @fn          createPacket
 *
 * @brief       This function is called before a packet is transmitted. It fills
 *              the txBuffer with a packet consisting of a length byte, two
 *              bytes packet counter and n random bytes.
 *
 *              The packet format is as follows:
 *              |--------------------------------------------------------------|
 *              |           |           |           |         |       |        |
 *              | pktLength | pktCount1 | pktCount0 | rndData |.......| rndData|
 *              |           |           |           |         |       |        |
 *              |--------------------------------------------------------------|
 *               txBuffer[0] txBuffer[1] txBuffer[2]  ......... txBuffer[PKT_LEN]
 *
 * @param       pointer to start of txBuffer
 *
 * @return      none
 */
void createPacket(uint8_t txBuffer[]){

    txBuffer[0] = PKT_LEN;                       // Length byte
    txBuffer[1] = 0x0F;       //(uint8_t) (packetCounter >> 8); // MSB of packetCounter
    txBuffer[2] = 0x05;       //(uint8_t)  packetCounter;       // LSB of packetCounter
  /*
  // Fill rest of buffer with random bytes
  uint8_t i;
  for(i =3; i< (PKT_LEN+1); i++)
  {
    txBuffer[i] = (uint8_t)rand();
  }*/
}
/*******************************************************************************
 *
 */
#pragma vector=PORTx_VECTOR
__interrupt void Port_isr(void)
{
    if (PACKET_SEND_RESIVE_IFG & PACKET_SEND_RESIVE_PIN1) {
        /* reset the RF_GDO_PIN */
        PACKET_SEND_RESIVE_IFG &= ~PACKET_SEND_RESIVE_PIN1;

        /* indicate button event */
        packetSemaphore = 1;
    }

    if (PACKET_SEND_RESIVE_IFG & PACKET_SEND_RESIVE_PIN2) {
        /* reset the RF_GDO_PIN */
        PACKET_SEND_RESIVE_IFG &= ~PACKET_SEND_RESIVE_PIN2;

        /* indicate button event */
        packetSemaphore1 = 1;
    }

//    __delay_cycles(400000);


}

