/*
 * brainx_sys.h
 *
 *  Created on: Feb 10, 2026
 *      Author: ADMIN
 */

#ifndef BRAINX_SYS_H_
#define BRAINX_SYS_H_

#include "main.h"

/* Power Management Board */
#define PWR_MNG_BOARD_ID 0x10
#define PWR_VOLTAGE_ID 0x01
#define PWR_CURRENT_ID 0x02
#define PWR_STATUS_ID 0x03
#define PWR_FAULT_ID 0x04

typedef enum{
	LOW = 0,
	OK = 1,
	CHARGING = 2
} BrainX_Power_Status;

/* IMU Sensor Board */
#define IMU_SENSOR_BOARD_ID 0x11
#define IMU_ANGLE_ID 0x01
#define IMU_FAULT_ID 0x02

/* Time-of-Flight Sensor Board */
#define TOF_SENSOR_BOARD_ID 0x12
#define TOF_DISTANCE_ID 0x01
#define TOF_FAULT_ID 0x02

/* Bumper Sensor Board */
#define BPR_SENSOR_BOARD_ID 0x14
#define BPR_STATUS_ID 0x01
#define BPR_FAULT_ID 0x02
#define BPR_MODE_ID 0x03

/*Bluetooth Gamepad */
#define BLT_GAMEPAD_ID 0x15
#define BLT_GAMEPAD_STATUS 0x01
#define BLT_GAMEPAD_FAULT 0x02

void BrainX_UpdateSystemInfo(uint8_t status);
void BrainX_DisplayLED();
void BrainX_LogInfo(void);


#endif /* INC_BRAINX_SYS_H_ */
