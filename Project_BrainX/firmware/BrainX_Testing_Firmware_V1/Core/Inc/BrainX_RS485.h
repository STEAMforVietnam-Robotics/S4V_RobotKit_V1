/*
 * BrainX_RS485.h
 *
 *  Created on: Jan 6, 2026
 *      Author: Manh Tien Vu
 */

#ifndef BRAINX_RS485_H_
#define BRAINX_RS485_H_

typedef struct {
	/* Receiving data buffer */
	uint8_t rx_buffer[10];

	/* Transmitting data buffer */
	uint8_t tx_buffer[10];

	/* State of the data analysis process */
	uint8_t state;

	/* Calculated checksum on the receiver side */
	uint8_t checksum;

	/* Device ID */
	uint8_t device_id;

	/* Verified data packet */
	uint8_t data;


} RS485_Device;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t DataSize);

void RS485_Init(RS485_Device *device, uint8_t id);
void RS485_RX_StateMachine(RS485_Device *device, uint8_t state);
void RS485_RX_ScanningHeader(RS485_Device *device);
void RS485_RX_CheckDevice(RS485_Device *device);
void RS485_RX_CheckDataContent(RS485_Device *device);
int RS485_RX_VerifyDataPacket(RS485_Device *device);
void RS485_RX_ReceiveVerifiedData(RS485_Device *device);
void RS485_RX_StartCommunication(RS485_Device *device);


#endif /* BRAINX_RS485_H_ */
