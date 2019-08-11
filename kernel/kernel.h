/*
 * kernel.h
 *
 * Created: 11.05.2019 21:15:24
 *  Author: ThePetrovich
 */ 

#ifndef KERNEL_H_
#define KERNEL_H_

#define KERNEL_VER "0.3.0-rc2"
#define KERNEL_TIMESTAMP __TIMESTAMP__

#define SDCARD_MOD_VER "0.0.4-bleeding"
#define SDCARD_MOD_TIMESTAMP __TIMESTAMP__

#include "types.h"
#include "hal.h"
#include "../tasks/tasks.h"
#include "kernel_config.h"
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ERR_QUEUE_OVERFLOW 1
#define ERR_QUEUE_END 2
#define ERR_WDT_RESET 3
#define ERR_BOD_RESET 4
#define ERR_KRN_RETURN 5
#define ERR_DEVICE_FAIL 6
	
#define PRIORITY_HIGH 0
#define PRIORITY_NORM 1
#define PRIORITY_LOW 2
	
#define KFLAG_INIT 0
#define KFLAG_TIMER_SET 1
#define KFLAG_TIMER_EN 2
#define KFLAG_TIMER_ISR 3
#define KFLAG_SD_INIT 4
#define KFLAG_LOG_SD 13
#define KFLAG_LOG_UART 14
#define KFLAG_DEBUG 15
	
#define KSTATE_ACTIVE 1
#define KSTATE_SUSPENDED 0

void kernel_setFlag(uint8_t flag, uint8_t value);
uint8_t kernel_checkFlag(uint8_t flag);
uint64_t kernel_getUptime();

uint8_t kernel_addCall(task t_ptr, uint8_t t_priority);
uint8_t kernel_addTask(task t_ptr, uint16_t t_period, uint8_t t_priority, uint8_t startupState);
uint8_t kernel_removeCall(uint8_t t_priority);
uint8_t kernel_removeTask(uint8_t position);
uint8_t kernel_setTaskState(task t_pointer, uint8_t state);
void kernel_clearTaskQueue();
void kernel_clearCallQueue(uint8_t t_priority);
void kernel_checkMCUCSR();
uint8_t kernelInit();

void kernel_stopTimer();
void kernel_startTimer();
uint8_t kernel_setTimer(timerISR t_pointer, uint32_t t_period);
uint8_t kernel_removeTimer(timerISR t_pointer);
void kernel_timerService();

#if KERNEL_SD_MODULE == 1
	void sd_puts(char * data);
	void sd_flush();
	void sd_readPtr();
	void sd_init();
#endif

#if KERNEL_WDT_MODULE == 1
	void wdt_saveMCUCSR(void) __attribute__((naked)) __attribute__((section(".init3")));
	//void wdt_disableWatchdog();
	void wdt_enableWatchdog();
#endif

#if KERNEL_UTIL_MODULE == 1
	void util_printVersion();
#endif

#if KERNEL_DEBUG_MODULE == 1
	#define DBG_MOD_VER "0.9.1-staging"
	#define DBG_MOD_TIMESTAMP __TIMESTAMP__
	#define UART_LOGGING 1
	#define PGM_ON 1
	#define PGM_OFF 0
	#define L_NONE 0
	#define L_INFO 1
	#define L_WARN 2
	#define L_ERROR 3
	#define L_FATAL 4
	void debug_sendMessage(uint8_t level, const char * format, va_list args);
	void debug_sendMessageSD(uint8_t level, const char * format, va_list args);
	void debug_sendMessage_p(uint8_t level, const char * format, va_list args);
	void debug_sendMessageSD_p(uint8_t level, const char * format, va_list args);
	void debug_logMessage(uint8_t pgm, uint8_t level, const char * format, ...);
#endif

#endif /* KERNEL_H_ */