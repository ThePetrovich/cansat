/*
 * drivers.h
 *
 * Created: 14.05.2019 19:24:18
 *  Author: ThePetrovich
 */ 

#ifndef DRIVERS_H_
#define DRIVERS_H_

#define SDCARD_DRV_VER "R0.0.2"
#define SDCARD_DRV_TIMESTAMP __TIMESTAMP__

#include "../drivers/uart.h"
#include "../drivers/spi.h"
#include "../drivers/twi.h"
#include "../drivers/adc.h"
#include "../drivers/devices/sensors/adxl345.h"
#include "../drivers/devices/sensors/bmp280.h"
#include "../drivers/devices/sensors/ds18b20.h"
#include "../drivers/devices/radio/nrf24.h"
#include "../external/pololu-driver/imuv3.h"
#include "../external/pff3a/pff.h"
#include "../external/pff3a/diskio.h"
#include "../external/nRF/nRF.h"

#endif /* DRIVERS_H_ */