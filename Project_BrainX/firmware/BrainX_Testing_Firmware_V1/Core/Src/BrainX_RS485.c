/*
 * BrainX_RS485.c
 *
 *  Created on: Jan 6, 2026
 *      Author: Manh Tien Vu
 */
#include "main.h"
#include "brainx_rs485.h"
#include <string.h>

void BrainX_RS485_Init(RS485_Device *device, uint8_t id,
				GPIO_TypeDef *GPIO_Driver_Enable_Port,
				uint16_t GPIO_Driver_Enable_Pin,
				GPIO_TypeDef *GPIO_Receiver_Enable_Port,
				uint16_t GPIO_Receiver_Enable_Pin)
{
	device->device_id = id;
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
	if(device->rx_buffer[0] == 0x55){
		device->target_device_id = device->rx_buffer[1];
		BrainX_RS485_RX_StateMachine(device, 1);
	}
}

void BrainX_RS485_RX_CheckDevice(RS485_Device *device){
	uint8_t device_list[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	uint8_t length;

	if(device->state == 1){
		length = sizeof(device_list) / sizeof(device_list[0]);
		for(int i = 0; i < length; i++){
			if(device->rx_buffer[1] == device_list[i]){
				BrainX_RS485_RX_StateMachine(device, 2);
				device-> target_device_id = device->rx_buffer[1];
				break;
			}
		}
	}

}

void BrainX_RS485_RX_CheckDataContent(RS485_Device *device){

	uint8_t CheckDataContent(uint8_t data_content_list[], uint8_t length){

		for(int i = 0; i < length; i++){
			if(device->rx_buffer[2] == data_content_list[i]){
				return 1;
			}
		}
		return 0;
	}

	if(device->state == 2){

		//Device ID (0x10): Power Management Board
		if(device->rx_buffer[1] == 0x10){
			uint8_t PWR_MNGT_BOARD_DATA_CONTENT[] = {0x01, 0x02, 0x03};

			if(CheckDataContent(PWR_MNGT_BOARD_DATA_CONTENT, 3) == 0){
				return;
			}

			//Data Content: Voltage Measurement 0 - 140 (0.00V to 14.0V)
			if(device->rx_buffer[2] == 0x01){
				//To Be Continued: Set a unique flag to log information via UART debugger
				BrainX_RS485_RX_StateMachine(device, 3);
			}

			//Data Content: Current Measurement 0 - 50 (0A to 50A)
			else if(device->rx_buffer[2] == 0x02){
				//To Be Continued: Set a unique flag to log information via UART debugger
				BrainX_RS485_RX_StateMachine(device, 3);
			}

			//Data Content: Power Status (LOW - OK - CHARGING)
			else if(device->rx_buffer[2] == 0x03){
				//To Be Continued: Set a unique flag to log information via UART debugger
				BrainX_RS485_RX_StateMachine(device, 3);
			}
		}

		//Device ID (0x11): IMU Sensor Board
		else if(device->rx_buffer[1] == 0x11){


		}
	}

}

int BrainX_RS485_RX_VerifyDataPacket(RS485_Device *device){

	if(device->state == 3){
		device->checksum = 0x55 | device->rx_buffer[1] | device->rx_buffer[2] | device->rx_buffer[3];

		if(device->checksum == device->rx_buffer[4]){
			return 1;
		}

	}
	return 0;
}

void BrainX_RS485_RX_ReceiveVerifiedData(RS485_Device *device){
	if(BrainX_RS485_RX_VerifyDataPacket(device) == 1){
		device->data_content = device->rx_buffer[2];
		device->data = device->rx_buffer[3];
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

void BrainX_RS485_TX_SendData(RS485_Device *device, UART_HandleTypeDef *huart, uint8_t target_device_id, uint8_t tx_buffer[10]){
	device->target_device_id = target_device_id;
	memcpy(device->tx_buffer, tx_buffer, 10);
	HAL_UART_Transmit(huart, device->tx_buffer, 10, 100);
}

