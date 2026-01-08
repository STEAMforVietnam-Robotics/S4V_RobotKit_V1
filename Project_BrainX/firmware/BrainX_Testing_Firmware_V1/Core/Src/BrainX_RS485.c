/*
 * BrainX_RS485.c
 *
 *  Created on: Jan 6, 2026
 *      Author: Manh Tien Vu
 */
#include "main.h"
#include "BrainX_RS485.h"

void RS485_Init(RS485_Device *device, uint8_t id){
	device->device_id = id;
}

void RS485_RX_StateMachine(RS485_Device *device, uint8_t state){
	device->state = state;
}

void RS485_RX_ScanningHeader(RS485_Device *device){
	if(device->rx_buffer[0] == 0x55){
		device->device_id = device->rx_buffer[1];
		RS485_RX_StateMachine(device, 1);
	}
}

void RS485_RX_CheckDevice(RS485_Device *device){
	uint8_t device_list[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	uint8_t length;

	if(device->state == 1){
		length = sizeof(device_list) / sizeof(device_list[0]);
		for(int i = 0; i < length; i++){
			if(device->rx_buffer[1] == device_list[i]){
				RS485_RX_StateMachine(device, 2);
				break;
			}
		}
	}

}

void RS485_RX_CheckDataContent(RS485_Device *device){

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
				RS485_RX_StateMachine(device, 3);
			}

			//Data Content: Current Measurement 0 - 50 (0A to 50A)
			else if(device->rx_buffer[2] == 0x02){
				//To Be Continued: Set a unique flag to log information via UART debugger
				RS485_RX_StateMachine(device, 3);
			}

			//Data Content: Power Status (LOW - OK - CHARGING)
			else if(device->rx_buffer[2] == 0x03){
				//To Be Continued: Set a unique flag to log information via UART debugger
				RS485_RX_StateMachine(device, 3);
			}
		}

		//Device ID (0x11): IMU Sensor Board
		else if(device->rx_buffer[1] == 0x11){


		}
	}

}

int RS485_RX_VerifyDataPacket(RS485_Device *device){

	if(device->state == 3){
		device->checksum = 0x55 | device->device_id | device->rx_buffer[2] | device->rx_buffer[3];

		if(device->checksum == device->rx_buffer[4]){
			return 1;
		}

	}
	return 0;
}

void RS485_RX_ReceiveVerifiedData(RS485_Device *device){
	if(RS485_RX_VerifyDataPacket(device) == 1){
		device->data = device->rx_buffer[3];
	}
}

void RS485_RX_StartCommunication(RS485_Device *device){
	RS485_RX_ScanningHeader(device); //Waiting for Protocol Header (0x55)
	RS485_RX_CheckDevice(device); //If header found, check next byte (device ID byte)
	RS485_RX_CheckDataContent(device); //If device ID is existent, verify if data content is existent
	RS485_RX_VerifyDataPacket(device); //Detect data errors
	RS485_RX_ReceiveVerifiedData(device);  //Process legitimate data
	RS485_RX_StateMachine(device, 0); //Reset State Machine to State 0
}

