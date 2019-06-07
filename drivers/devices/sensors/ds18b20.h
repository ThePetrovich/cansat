/*
 * ds18b20.h
 *
 * Created: 17.05.2019 17:58:22
 *  Author: Admin
 */ 


#ifndef DS18B20_H_
#define DS18B20_H_

#define DS18DRV_VER "0.0.1-bleeding"
#define DS18DRV_TIMESTAMP __TIMESTAMP__

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/common.h>
#include <avr/interrupt.h>

#define DS_CMD_CONVERTTEMP 0x44
#define DS_CMD_RSCRATCHPAD 0xBE
#define DS_CMD_WSCRATCHPAD 0x4E
#define DS_CMD_CPYSCRATCHPAD 0x48
#define DS_CMD_RECEEPROM 0xB8
#define DS_CMD_RPWRSUPPLY 0xB4
#define DS_CMD_SEARCHROM 0xF0
#define DS_CMD_READROM 0x33
#define DS_CMD_MATCHROM 0x55
#define DS_CMD_SKIPROM 0xCC
#define DS_CMD_ALARMSEARCH 0xEC

#define DS18B20_PORT PORTA
#define DS18B20_DDR DDRA
#define DS18B20_PIN PINA
#define DS18B20_IO 1

#define DS_RESOLUTION_12BIT 625

float ds18b20_readTemperature(void);

#endif /* DS18B20_H_ */