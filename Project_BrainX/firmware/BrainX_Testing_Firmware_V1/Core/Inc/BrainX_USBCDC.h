/*
 * BrainX_USBCDC.h
 *
 *  Created on: Jan 7, 2026
 *      Author: Manh Tien Vu
 */

#include <main.h>

#ifndef BRAINX_USBCDC_H_
#define BRAINX_USBCDC_H_

#define HEADER_BYTE 0x55

#define RX_BUFF_LEN 10

void BrainX_USBCDC_RX_StateMachine(uint8_t state);
void BrainX_USBCDC_RX_ScanningHeaderByte();
void BrainX_USBCDC_RX_CheckDevice();
void BrainX_USBCDC_RX_CheckDataContent();
int BrainX_USBCDC_RX_VerifyDataPacket();
void BrainX_USBCDC_RX_ReceiveVerifiedData();
void BrainX_USBCDC_RX_ReceiveData();

void BrainX_USBCDC_TX_TransmitData();


#endif /* INC_BRAINX_USBCDC_H_ */
