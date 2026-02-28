/*
 * mtdriver_rs485.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#ifndef INC_MTDRIVER_RS485_H_
#define INC_MTDRIVER_RS485_H_

#include <main.h>


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
	uint8_t rxBuffer[10];

	/* Transmitting data buffer */
	uint8_t txBuffer[10];

	/* State of the data analysis process */
	uint8_t state;

	/* Calculated checksum on the receiver side */
	uint8_t checksum;

	/* Device ID */
	uint8_t deviceID;

	/* Talking to WHO (target Device ID */
	uint8_t targetDeviceID;

	/* Verified data content */
	uint8_t dataContent;

	/* Verified data packet */
	uint8_t data;


} RS485_Device;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t DataSize);

void MTDRIVER_RS485_Init(uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin);

void MTDRIVER_RS485_RX_Enable();
void MTDRIVER_RS485_RX_StateMachine(uint8_t state);
void MTDRIVER_RS485_RX_ScanningHeader();
void MTDRIVER_RS485_RX_CheckDevice();
void MTDRIVER_RS485_RX_CheckDataContent();
int MTDRIVER_RS485_RX_VerifyDataPacket();
void MTDRIVER_RS485_RX_ReceiveVerifiedData();
void MTDRIVER_RS485_RX_ReceiveData();

void MTDRIVER_RS485_TX_Enable();
void MTDRIVER_RS485_TX_SendData(UART_HandleTypeDef *huart, uint8_t targetDeviceID, uint8_t TxBuffer[10]);




#endif /* INC_MTDRIVER_RS485_H_ */
