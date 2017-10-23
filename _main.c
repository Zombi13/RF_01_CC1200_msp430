
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


int main(void) {


    trxRfSpiInterfaceInit(2);
    UART_init();
    port_init ();

    // Write radio registers
    registerConfig();

    __delay_cycles(100);    //Important!!! need to reconfigure port


    while(TRUE){

        switch (worck_mod){
        case RF_Set_mod:{


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

                packetSemaphore = ISR_ACTION_REQUIRED;

                __delay_cycles(100);    //Important!!! need to reconfigure port

            }
            if (worck_mod == RF_RX_mod || worck_mod == 0x0C){ //RF_RX

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

