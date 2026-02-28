/*
 * mtdriver_sys.c
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#include <mtdriver_sys.h>
#include <main.h>
#include <tim.h>

extern TIM_HandleTypeDef htim2;
//Set all motorDriver paramaters default as 0x00
MotorDriver motorDriver = {0x00, 0x00, 0x00, 0x00, &htim2};

void MTDRIVER_UpdateSystemInfo(uint16_t id, uint8_t pwm, uint8_t mode, uint8_t fault){
	motorDriver.fault = fault;
	if(motorDriver.fault != 0x00){
		Error_Handler();
	}

	motorDriver.uniqueBoardID = id;
	motorDriver.pwm = pwm*10;
	motorDriver.mode = mode;
}

void MTDRIVER_Control(void){
	switch(motorDriver.mode){
		case COAST_MODE:
			motorDriver.htim->Instance->CCR3 = 0;
			break;
		case BRAKE_MODE:
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 0);
			HAL_GPIO_WritePin(INA_GPIO_Port, INA_Pin, 0);
			motorDriver.htim->Instance->CCR3 = motorDriver.pwm;
			break;

	}
}

