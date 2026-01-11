/*
 * BrainX_RS485.h
 *
 *  Created on: Jan 6, 2026
 *      Author: Manh Tien Vu
 */

#ifndef BRAINX_RS485_H_
#define BRAINX_RS485_H_

typedef struct {

	/* GPIO Port: Driver Enable GPIO Pin Port */
	GPIO_TypeDef* GPIO_Driver_Enable_Port;

	/* DE: Driver Enable pin */
	uint16_t GPIO_Driver_Enable_Pin;

	/* GPIO Port: Receiver Enable GPIO Pin Port */
	GPIO_TypeDef* GPIO_Receiver_Enable_Port;

	/* RE: Receiver Enable pin */
	uint16_t GPIO_Receiver_Enable_Pin;

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

	/* Talking to WHO (target Device ID */
	uint8_t target_device_id;

	/* Verified data content */
	uint8_t data_content;

	/* Verified data packet */
	uint8_t data;


} RS485_Device;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t DataSize);

void RS485_Init(RS485_Device *device, uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin);

void RS485_RX_Enable(RS485_Device *device);
void RS485_RX_StateMachine(RS485_Device *device, uint8_t state);
void RS485_RX_ScanningHeader(RS485_Device *device);
void RS485_RX_CheckDevice(RS485_Device *device);
void RS485_RX_CheckDataContent(RS485_Device *device);
int RS485_RX_VerifyDataPacket(RS485_Device *device);
void RS485_RX_ReceiveVerifiedData(RS485_Device *device);
void RS485_RX_ReceiveData(RS485_Device *device);

void RS485_TX_Enable(RS485_Device *device);
void RS485_TX_SendData(RS485_Device *device, UART_HandleTypeDef *huart, uint8_t target_device_id, uint8_t tx_buffer[10]);


#endif /* BRAINX_RS485_H_ */
