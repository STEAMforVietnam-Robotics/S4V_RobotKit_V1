/*
 * mtdriver_rs485.c
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#include "mtdriver_rs485.h"
#include "mtdriver_sys.h"
#include <string.h>

RS485_Device rs485Port;
uint8_t rs485RxOkFlag = 0;

void MTDRIVER_RS485_Init(uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin)
{
	rs485Port.deviceID = id;
	rs485Port.GPIO_Driver_Enable_Port = GPIO_Driver_Enable_Port;
	rs485Port.GPIO_Driver_Enable_Pin = GPIO_Driver_Enable_Pin;
	rs485Port.GPIO_Receiver_Enable_Port = GPIO_Receiver_Enable_Port;
	rs485Port.GPIO_Receiver_Enable_Pin = GPIO_Receiver_Enable_Pin;
}

void MTDRIVER_RS485_RX_Enable(){
	/* RECEIVING MODE: RE: 0, DE: 0 */
	HAL_GPIO_WritePin(rs485Port.GPIO_Receiver_Enable_Port, rs485Port.GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(rs485Port.GPIO_Driver_Enable_Port, rs485Port.GPIO_Driver_Enable_Pin, 0);
}

void MTDRIVER_RS485_RX_StateMachine(uint8_t state){
	rs485Port.state = state;
}

void MTDRIVER_RS485_RX_ScanningHeader(){
	if(rs485Port.rxBuffer[0] == 0x55){
		MTDRIVER_RS485_RX_StateMachine(1);
	}
}

void MTDRIVER_RS485_RX_CheckDevice(){

	if(rs485Port.state == 1){
			if(rs485Port.rxBuffer[1] == MT_DRIVER_ID){
				rs485Port.targetDeviceID = rs485Port.rxBuffer[1];
				MTDRIVER_RS485_RX_StateMachine(2);
			}
	}
}


void MTDRIVER_RS485_RX_CheckDataContent(){

	uint8_t CheckDataContent(uint8_t dataContentList[], uint8_t length){

		for(int i = 0; i < length; i++){
			if(rs485Port.rxBuffer[2] == dataContentList[i]){
				return 1;
			}
		}
		return 0;
	}

	if(rs485Port.state == 2){

		if(rs485Port.rxBuffer[1] == MT_DRIVER_ID){
			uint8_t MT_DRIVER_DATA_CONTENT[] = {0x01, 0x02, 0x03, 0x04};
			if(CheckDataContent(MT_DRIVER_DATA_CONTENT, 4) == 0){
				return;
			}
			switch(rs485Port.rxBuffer[2]){
				case 0x01:
					MTDRIVER_RS485_RX_StateMachine(3);
					break;
				case 0x02:
					MTDRIVER_RS485_RX_StateMachine(3);
					break;
				case 0x03:
					MTDRIVER_RS485_RX_StateMachine(3);
					break;
				case 0x04:
					MTDRIVER_RS485_RX_StateMachine(3);
					break;
			}

		}
	}

}

int MTDRIVER_RS485_RX_VerifyDataPacket(){

	if(rs485Port.state == 3){
		rs485Port.checksum = 0x55 | rs485Port.rxBuffer[1] | rs485Port.rxBuffer[2] | rs485Port.rxBuffer[3];

		if(rs485Port.checksum == rs485Port.rxBuffer[4]){
			return 1;
		}

	}
	return 0;
}

void MTDRIVER_RS485_RX_ReceiveVerifiedData(){
	if(MTDRIVER_RS485_RX_VerifyDataPacket() == 1){
		rs485Port.dataContent = rs485Port.rxBuffer[2];
		rs485Port.data = rs485Port.rxBuffer[3];
		rs485RxOkFlag = 1;
	}
}

void MTDRIVER_RS485_RX_ReceiveData(){
	MTDRIVER_RS485_RX_ScanningHeader(); //Waiting for Protocol Header (0x55)
	MTDRIVER_RS485_RX_CheckDevice(); //If header found, check next byte (device ID byte)
	MTDRIVER_RS485_RX_CheckDataContent(); //If device ID is existent, verify if data content is existent
	MTDRIVER_RS485_RX_VerifyDataPacket(); //Detect data errors
	MTDRIVER_RS485_RX_ReceiveVerifiedData();  //Process legitimate data
	MTDRIVER_RS485_RX_StateMachine(0); //Reset State Machine to State 0
}

void MTDRIVER_RS485_TX_Enable(){
	/* TRANSMITTING MODE: RE: X (Don't care), DE: 1 */
	HAL_GPIO_WritePin(rs485Port.GPIO_Receiver_Enable_Port, rs485Port.GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(rs485Port.GPIO_Driver_Enable_Port, rs485Port.GPIO_Driver_Enable_Pin, 1);
}

void MTDRIVER_RS485_TX_SendData(UART_HandleTypeDef *huart, uint8_t targetDeviceID, uint8_t txBuffer[10]){
	rs485Port.targetDeviceID = targetDeviceID;
	memcpy(rs485Port.txBuffer, txBuffer, 10);
	HAL_UART_Transmit(huart, rs485Port.txBuffer, 10, 100);
}
