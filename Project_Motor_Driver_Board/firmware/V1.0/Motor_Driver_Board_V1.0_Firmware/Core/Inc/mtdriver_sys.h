/*
 * mtdriver_sys.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#ifndef INC_MTDRIVER_SYS_H_
#define INC_MTDRIVER_SYS_H_

#include "stdint.h"
#include "tim.h"
/*Motor Driver Board */
#define MT_DRIVER_ID 0x15
#define MT_DRIVER_UNIQUE_BOARD_ID 0x01
#define MT_DRIVER_PWM_ID 0x02
#define MT_DRIVER_MODE_ID 0x03
#define MT_DRIVER_FAULT_ID 0x04

#define COAST_MODE 0x01
#define BRAKE_MODE 0x02

typedef struct{
	uint16_t uniqueBoardID;
	uint8_t pwm;
	uint8_t mode;
	uint8_t fault;
	TIM_HandleTypeDef* htim;
}
MotorDriver;

void MTDRIVER_UpdateSystemInfo(uint16_t id, uint8_t pwm, uint8_t mode, uint8_t fault);
void MTDRIVER_Control(void);
void MTDRIVER_DisplayLED(void);
void MTDRIVER_LogInfo(void);

#endif /* INC_MTDRIVER_SYS_H_ */
