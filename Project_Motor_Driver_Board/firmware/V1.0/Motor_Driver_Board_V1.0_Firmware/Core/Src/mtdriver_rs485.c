/*
 * mtdriver_rs485.c
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#include "mtdriver_rs485.h"
#include "mtdriver_sys.h"
#include <string.h>

RS485_Device mtDriverBoard;

void MTDRIVER_RS485_Init(uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin)
{
	mtDriverBoard.deviceID = id;
	mtDriverBoard.GPIO_Driver_Enable_Port = GPIO_Driver_Enable_Port;
	mtDriverBoard.GPIO_Driver_Enable_Pin = GPIO_Driver_Enable_Pin;
	mtDriverBoard.GPIO_Receiver_Enable_Port = GPIO_Receiver_Enable_Port;
	mtDriverBoard.GPIO_Receiver_Enable_Pin = GPIO_Receiver_Enable_Pin;
}

void MTDRIVER_RS485_RX_Enable(){
	/* RECEIVING MODE: RE: 0, DE: 0 */
	HAL_GPIO_WritePin(mtDriverBoard.GPIO_Receiver_Enable_Port, mtDriverBoard.GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(mtDriverBoard.GPIO_Driver_Enable_Port, mtDriverBoard.GPIO_Driver_Enable_Pin, 0);
}

void MTDRIVER_RS485_RX_StateMachine(uint8_t state){
	mtDriverBoard.state = state;
}

void MTDRIVER_RS485_RX_ScanningHeader(){
	if(mtDriverBoard.rxBuffer[0] == 0x55){
		MTDRIVER_RS485_RX_StateMachine(1);
	}
}

void MTDRIVER_RS485_RX_CheckDevice(){

	if(mtDriverBoard.state == 1){
			if(mtDriverBoard.rxBuffer[1] == MT_DRIVER_ID){
				mtDriverBoard.targetDeviceID = mtDriverBoard.rxBuffer[1];
				MTDRIVER_RS485_RX_StateMachine(2);
			}
	}
}


void MTDRIVER_RS485_RX_CheckDataContent(){

	uint8_t CheckDataContent(uint8_t dataContentList[], uint8_t length){

		for(int i = 0; i < length; i++){
			if(mtDriverBoard.rxBuffer[2] == dataContentList[i]){
				return 1;
			}
		}
		return 0;
	}

	if(mtDriverBoard.state == 2){

		if(mtDriverBoard.rxBuffer[1] == MT_DRIVER_ID){
			uint8_t MT_DRIVER_DATA_CONTENT[] = {0x01, 0x02, 0x03, 0x04};
			if(CheckDataContent(MT_DRIVER_DATA_CONTENT, 4) == 0){
				return;
			}
			switch(mtDriverBoard.rxBuffer[2]){
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

	if(mtDriverBoard.state == 3){
		mtDriverBoard.checksum = 0x55 | mtDriverBoard.rxBuffer[1] | mtDriverBoard.rxBuffer[2] | mtDriverBoard.rxBuffer[3];

		if(mtDriverBoard.checksum == mtDriverBoard.rxBuffer[4]){
			return 1;
		}

	}
	return 0;
}

void MTDRIVER_RS485_RX_ReceiveVerifiedData(){
	if(MTDRIVER_RS485_RX_VerifyDataPacket() == 1){
		mtDriverBoard.dataContent = mtDriverBoard.rxBuffer[2];
		mtDriverBoard.data = mtDriverBoard.rxBuffer[3];
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
	HAL_GPIO_WritePin(mtDriverBoard.GPIO_Receiver_Enable_Port, mtDriverBoard.GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(mtDriverBoard.GPIO_Driver_Enable_Port, mtDriverBoard.GPIO_Driver_Enable_Pin, 1);
}

void MTDRIVER_RS485_TX_SendData(UART_HandleTypeDef *huart, uint8_t targetDeviceID, uint8_t txBuffer[10]){
	mtDriverBoard.targetDeviceID = targetDeviceID;
	memcpy(mtDriverBoard.txBuffer, txBuffer, 10);
	HAL_UART_Transmit(huart, mtDriverBoard.txBuffer, 10, 100);
}




