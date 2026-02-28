/*
 * mtdriver_sys.h
 *
 *  Created on: Feb 28, 2026
 *      Author: Manh Vu Tien
 */

#ifndef INC_MTDRIVER_SYS_H_
#define INC_MTDRIVER_SYS_H_

/*Motor Driver Board */
#define MT_DRIVER_ID 0x15
#define MT_DRIVER_UNIQUE_BOARD_ID 0x01
#define MT_DRIVER_PWM_ID 0x02
#define MT_DRIVER_MODE_ID 0x03
#define MT_DRIVER_FAULT_ID 0x04

void MTDRIVER_UpdateSystemInfo(uint8_t status);
void MTDRIVER_DisplayLED();
void MTDRIVER_LogInfo(void);

#endif /* INC_MTDRIVER_SYS_H_ */
