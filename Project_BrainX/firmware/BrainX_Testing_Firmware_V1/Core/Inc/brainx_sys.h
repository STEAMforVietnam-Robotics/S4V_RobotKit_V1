/*
 * brainx_sys.h
 *
 *  Created on: Feb 10, 2026
 *      Author: ADMIN
 */

#ifndef BRAINX_SYS_H_
#define BRAINX_SYS_H_

#include "main.h"

typedef enum{
	LOW = 0,
	OK = 1,
	CHARGING = 2
} BrainX_Power_Status;

void BrainX_DisplayLED(uint8_t status);
void BrainX_LogInfo(void);


#endif /* INC_BRAINX_SYS_H_ */
