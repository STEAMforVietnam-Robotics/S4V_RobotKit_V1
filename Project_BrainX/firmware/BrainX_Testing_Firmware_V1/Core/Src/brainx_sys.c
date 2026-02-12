/*
 * brainx_sys.c
 *
 *  Created on: Feb 10, 2026
 *      Author: ADMIN
 */

#include "brainx_sys.h"
#include "tim.h"

BrainX_Power_Status brainxPowerStatus = 5;

void BrainX_UpdateSystemInfo(uint8_t status){
	brainxPowerStatus = status;
}

void BrainX_DisplayLED(){
	switch(brainxPowerStatus){
		case LOW:
			HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, 1);
			HAL_GPIO_WritePin(CRG_GPIO_Port, CRG_Pin, 1);
			HAL_GPIO_TogglePin(LOW_GPIO_Port, LOW_Pin);
			break;
		case CHARGING:
			HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, 1);
			HAL_GPIO_WritePin(CRG_GPIO_Port, CRG_Pin, 0);
			HAL_GPIO_WritePin(LOW_GPIO_Port, LOW_Pin, 1);
			break;
		case OK:
			HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, 0);
			HAL_GPIO_WritePin(CRG_GPIO_Port, CRG_Pin, 1);
			HAL_GPIO_WritePin(LOW_GPIO_Port, LOW_Pin, 1);
			break;
	}
}

void BrainX_LogInfo(){

}
