/*
 * BrainX_USBCDC.c
 *
 *  Created on: Jan 7, 2026
 *      Author: Manh Tien Vu
 */


#include <brainx_usbcdc.h>

uint8_t usbcdcRxBuff[RX_BUFF_LEN];
uint8_t usbcdcTxState = 0;
uint8_t usbcdcTxFlag = 0;

uint8_t usbcdcRxBuffer[RX_BUFF_LEN];
uint8_t usbcdcRxState = 0;
uint8_t usbcdcRxFlag = 0;
uint8_t usbcdcRxOkFlag = 0;

uint8_t deviceID;
uint8_t dataContent;
uint8_t data;
uint8_t checksum;

void BrainX_USBCDC_RX_StateMachine(uint8_t state){
	usbcdcRxState = state;
}

void BrainX_USBCDC_RX_ScanningHeader(){
	if(usbcdcRxBuffer[0] == HEADER_BYTE){
		BrainX_USBCDC_RX_StateMachine(1);
	}
}

void BrainX_USBCDC_RX_CheckDevice(){
	uint8_t deviceList[] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
	uint8_t length;

	if(usbcdcRxState == 1){
		length = sizeof(deviceList) / sizeof(deviceList[0]);
		for(int i = 0; i < length; i++){
			if(usbcdcRxBuffer[1] == deviceList[i]){
				BrainX_USBCDC_RX_StateMachine(2);
				break;
			}
		}
	}

}

void BrainX_USBCDC_RX_CheckDataContent(){

	uint8_t CheckDataContent(uint8_t dataContentList[], uint8_t length){

		for(int i = 0; i < length; i++){
			if(usbcdcRxBuffer[2] == dataContentList[i]){
				return 1;
			}
		}
		return 0;
	}

	if(usbcdcRxState == 2){

		//Device ID (0x10): Power Management Board
		if(usbcdcRxBuffer[1] == 0x10){
			uint8_t PWR_MNGT_BOARD_DATA_CONTENT[] = {0x01, 0x02, 0x03};

			if(CheckDataContent(PWR_MNGT_BOARD_DATA_CONTENT, 3) == 0){
				return;
			}

			switch(usbcdcRxBuffer[2]){
			//Data Content: Voltage Measurement 0 - 140 (0.00V to 14.0V)
				case 0x01:
				//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_USBCDC_RX_StateMachine(3);
					break;

			//Data Content: Current Measurement 0 - 50 (0A to 50A)
				case 0x02:
				//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_USBCDC_RX_StateMachine(3);
					break;

			//Data Content: Power Status (LOW - OK - CHARGING)
				case 0x03:
				//To Be Continued: Set a unique flag to log information via UART debugger
					BrainX_USBCDC_RX_StateMachine(3);
					break;
			}


		}
	}
}

int BrainX_USBCDC_RX_VerifyDataPacket(){

	if(usbcdcRxState == 3){
		checksum = 0x55 | usbcdcRxBuffer[1] | usbcdcRxBuffer[2] | usbcdcRxBuffer[3];

		if(checksum == usbcdcRxBuffer[4]){
			return 1;
		}

	}
	return 0;
}

void BrainX_USBCDC_RX_ReceiveVerifiedData(){

	if(BrainX_USBCDC_RX_VerifyDataPacket() == 1){
		usbcdcRxOkFlag = 1;
		dataContent = usbcdcRxBuffer[2];
		data = usbcdcRxBuffer[3];
	}

}

void BrainX_USBCDC_RX_ReceiveData(){
	BrainX_USBCDC_RX_ScanningHeader(); //Waiting for Protocol Header (0x55)
	BrainX_USBCDC_RX_CheckDevice(); //If header found, check next byte (device ID byte)
	BrainX_USBCDC_RX_CheckDataContent(); //If device ID is existent, verify if data content is existent
	BrainX_USBCDC_RX_VerifyDataPacket(); //Detect data errors
	BrainX_USBCDC_RX_ReceiveVerifiedData();  //Process legitimate data
	BrainX_USBCDC_RX_StateMachine(0); //Reset State Machine to State 0
}
