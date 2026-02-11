/*
 * BrainX_USBCDC.h
 *
 *  Created on: Jan 7, 2026
 *      Author: Manh Tien Vu
 */

#include <main.h>

#ifndef BRAINX_USBCDC_H_
#define BRAINX_USBCDC_H_

#define HEADER_BYTE 0x55

#define RX_BUFF_LEN 10

/* Power Management Board */
#define PWR_MNG_BOARD_ID 0x10
#define PWR_VOLTAGE_ID 0x01
#define PWR_CURRENT_ID 0x02
#define PWR_STATUS_ID 0x03
#define PWR_FAULT_ID 0x04

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


void BrainX_USBCDC_RX_StateMachine(uint8_t state);
void BrainX_USBCDC_RX_ScanningHeaderByte();
void BrainX_USBCDC_RX_CheckDevice();
void BrainX_USBCDC_RX_CheckDataContent();
int BrainX_USBCDC_RX_VerifyDataPacket();
void BrainX_USBCDC_RX_ReceiveVerifiedData();
void BrainX_USBCDC_RX_ReceiveData();


#endif /* INC_BRAINX_USBCDC_H_ */
