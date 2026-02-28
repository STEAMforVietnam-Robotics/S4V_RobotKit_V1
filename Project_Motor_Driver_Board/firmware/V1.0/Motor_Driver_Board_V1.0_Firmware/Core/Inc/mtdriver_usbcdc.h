/*
 * mtdriver_usbcdc.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */
#include <main.h>

#ifndef INC_MTDRIVER_USBCDC_H_
#define INC_MTDRIVER_USBCDC_H_


#define HEADER_BYTE 0x55
#define RX_BUFF_LEN 10

void MTDRIVER_USBCDC_RX_StateMachine(uint8_t state);
void MTDRIVER_USBCDC_RX_ScanningHeaderByte();
void MTDRIVER_USBCDC_RX_CheckDevice();
void MTDRIVER_USBCDC_RX_CheckDataContent();
int MTDRIVER_USBCDC_RX_VerifyDataPacket();
void MTDRIVER_USBCDC_RX_ReceiveVerifiedData();
void MTDRIVER_USBCDC_RX_ReceiveData();

void MTDRIVER_USBCDC_TX_TransmitData();



#endif /* INC_MTDRIVER_USBCDC_H_ */
