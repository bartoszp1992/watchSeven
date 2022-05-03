/*
 * backup.h
 *
 *  Created on: 2 maj 2022
 *      Author: bartosz
 */

#ifndef BACKUP_H_
#define BACKUP_H_

#include <stdint.h>
#include "eeprom_emul.h"
#include "rtc.h"


//USER CONFIGURATION
#define BACKUP_VIRTUAL_ADRESS 1 //virtual eeprom address for store time- first of two


uint8_t eeStatus;
extern void Error_Handler(void);

void backupInit(void);
void backupRestore(RTCChronoTypeDef *chrono);
void backupWrite(RTCChronoTypeDef *chrono);

#endif /* BACKUP_H_ */
