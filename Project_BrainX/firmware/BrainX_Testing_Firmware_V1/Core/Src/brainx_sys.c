/*
 * brainx_sys.c
 *
 *  Created on: Feb 10, 2026
 *      Author: ADMIN
 */

#include "brainx_sys.h"

BrainX_Power_Status brainx_power_status;

void BrainX_DisplayLED(uint8_t status){
	switch(status){
		case LOW:
			HAL_GPIO_WritePin(OK_GPIO_Port, OK_Pin, 1);
			HAL_GPIO_WritePin(CRG_GPIO_Port, CRG_Pin, 1);
			HAL_GPIO_WritePin(LOW_GPIO_Port, LOW_Pin, 0);
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
