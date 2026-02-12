/*
 * BrainX_RS485.c
 *
 *  Created on: Jan 6, 2026
 *      Author: Manh Tien Vu
 */
#include "brainx_rs485.h"
#include "brainx_sys.h"
#include <string.h>

void BrainX_RS485_Init(RS485_Device *device, uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin)
{
	device->deviceID = id;
	device->GPIO_Driver_Enable_Port = GPIO_Driver_Enable_Port;
	device->GPIO_Driver_Enable_Pin = GPIO_Driver_Enable_Pin;
	device->GPIO_Receiver_Enable_Port = GPIO_Receiver_Enable_Port;
	device->GPIO_Receiver_Enable_Pin = GPIO_Receiver_Enable_Pin;
}

void BrainX_RS485_RX_Enable(RS485_Device *device){
	/* RECEIVING MODE: RE: 0, DE: 0 */
	HAL_GPIO_WritePin(device->GPIO_Receiver_Enable_Port, device->GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(device->GPIO_Driver_Enable_Port, device->GPIO_Driver_Enable_Pin, 0);
}

void BrainX_RS485_RX_StateMachine(RS485_Device *device, uint8_t state){
	device->state = state;
}

void BrainX_RS485_RX_ScanningHeader(RS485_Device *device){
	if(device->rxBuffer[0] == 0x55){
		BrainX_RS485_RX_StateMachine(device, 1);
	}
}

void BrainX_RS485_RX_CheckDevice(RS485_Device *device){
	uint8_t deviceList[] = {PWR_MNG_BOARD_ID, IMU_SENSOR_BOARD_ID, TOF_SENSOR_BOARD_ID, BPR_SENSOR_BOARD_ID, BLT_GAMEPAD_ID};
	uint8_t length;

	if(device->state == 1){
		length = sizeof(deviceList) / sizeof(deviceList[0]);
		for(int i = 0; i < length; i++){
			if(device->rxBuffer[1] == deviceList[i]){
				device-> targetDeviceID = device->rxBuffer[1];
				BrainX_RS485_RX_StateMachine(device, 2);
				break;
			}
		}
	}

}

void BrainX_RS485_RX_CheckDataContent(RS485_Device *device){

	uint8_t CheckDataContent(uint8_t dataContentList[], uint8_t length){

		for(int i = 0; i < length; i++){
			if(device->rxBuffer[2] == dataContentList[i]){
				return 1;
			}
		}
		return 0;
	}

	if(device->state == 2){

		//Device ID (0x10): Power Management Board
		if(device->rxBuffer[1] == PWR_MNG_BOARD_ID){
			uint8_t PWR_MNGT_BOARD_DATA_CONTENT[] = {PWR_VOLTAGE_ID, PWR_CURRENT_ID, PWR_STATUS_ID, PWR_FAULT_ID};

			if(CheckDataContent(PWR_MNGT_BOARD_DATA_CONTENT, 4) == 0){
				return;
			}

			switch(device->rxBuffer[2]){
				//Data Content: Voltage Measurement 0 - 140 (0.00V to 14.0V)
				case PWR_VOLTAGE_ID:
					//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_RS485_RX_StateMachine(device, 3);
					break;

				//Data Content: Current Measurement 0 - 50 (0A to 50A)
				case PWR_CURRENT_ID:
					//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_RS485_RX_StateMachine(device, 3);
					break;

				//Data Content: Power Status (LOW - OK - CHARGING)
				case PWR_STATUS_ID:
					//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_RS485_RX_StateMachine(device, 3);
					break;
			}
		}

		//Device ID (0x11): IMU Sensor Board
		else if(device->rxBuffer[1] == IMU_SENSOR_BOARD_ID){


		}
	}

}

int BrainX_RS485_RX_VerifyDataPacket(RS485_Device *device){

	if(device->state == 3){
		device->checksum = 0x55 | device->rxBuffer[1] | device->rxBuffer[2] | device->rxBuffer[3];

		if(device->checksum == device->rxBuffer[4]){
			return 1;
		}

	}
	return 0;
}

void BrainX_RS485_RX_ReceiveVerifiedData(RS485_Device *device){
	if(BrainX_RS485_RX_VerifyDataPacket(device) == 1){
		device->dataContent = device->rxBuffer[2];
		device->data = device->rxBuffer[3];
	}
}

void BrainX_RS485_RX_ReceiveData(RS485_Device *device){
	BrainX_RS485_RX_ScanningHeader(device); //Waiting for Protocol Header (0x55)
	BrainX_RS485_RX_CheckDevice(device); //If header found, check next byte (device ID byte)
	BrainX_RS485_RX_CheckDataContent(device); //If device ID is existent, verify if data content is existent
	BrainX_RS485_RX_VerifyDataPacket(device); //Detect data errors
	BrainX_RS485_RX_ReceiveVerifiedData(device);  //Process legitimate data
	BrainX_RS485_RX_StateMachine(device, 0); //Reset State Machine to State 0
}

void BrainX_RS485_TX_Enable(RS485_Device *device){
	/* TRANSMITTING MODE: RE: X (Don't care), DE: 1 */
	HAL_GPIO_WritePin(device->GPIO_Receiver_Enable_Port, device->GPIO_Receiver_Enable_Pin, 0);
	HAL_GPIO_WritePin(device->GPIO_Driver_Enable_Port, device->GPIO_Driver_Enable_Pin, 1);
}

void BrainX_RS485_TX_SendData(RS485_Device *device, UART_HandleTypeDef *huart, uint8_t targetDeviceID, uint8_t txBuffer[10]){
	device->targetDeviceID = targetDeviceID;
	memcpy(device->txBuffer, txBuffer, 10);
	HAL_UART_Transmit(huart, device->txBuffer, 10, 100);
}

