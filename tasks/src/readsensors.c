/*
 * readsensors.c
 *
 * Created: 16.06.2019 0:49:48
 *  Author: ThePetrovich
 */ 

#include "../tasks.h"

uint16_t altitude_init;
uint16_t altitude = 0;

float convertToDecimal(float lat);

void readBMP(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Reading BMP280\r\n"));
	}
	int16_t t2, alt;
	int32_t prs;
	t2 = bmp280_readTemperature();
	prs = bmp280_readPressure();
	alt = bmp280_calcAltitude(101325);
	altitude = (uint16_t)alt;
	sprintf(packetMain.t2, "T2=%d;", t2);
	sprintf(packetMain.prs, "PRS=%ld;", prs);
	sprintf(packetMain.alt, "ALT=%d;", alt);
	sprintf(packetGPS.alt, "ALT=%d;", alt);
	kernel_addTask(readBMP, 200, PRIORITY_MID);
	wdt_reset();
}

void readADXL(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Reading ADXL345\r\n"));
	}
	int16_t ax, ay, az;
	ax = adxl345_readX();
	ay = adxl345_readY();
	az = adxl345_readZ();
	sprintf(packetOrient.ax, "AX=%d;", ax*10);
	sprintf(packetOrient.ay, "AY=%d;", ay*10);
	sprintf(packetOrient.az, "AZ=%d;", az*10);
	kernel_addTask(readADXL, 100, PRIORITY_MID);
	wdt_reset();
}

void requestDS18(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Requesting temperature from DS18B20\r\n"));
	}
	ds18b20_requestTemperature();
	kernel_addTask(readDS18, 750, PRIORITY_MID);
	kernel_addTask(requestDS18, 760, PRIORITY_MID);
	wdt_reset();
}

void readDS18(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Reading DS18B20\r\n"));
	}
	char * t1 = ds18b20_readTemperature();
	float t1_conv = atof(t1);
	sprintf(packetMain.t1, "T1=%d;", (int)(t1_conv*10.0));
	//debug_logMessage(packetMain.t1, 1, 0);
	wdt_reset();
}
//Holy shiet
void readIMU(){
	
}

void readGPS(){
	if(kernel_checkFlag(KFLAG_DEBUG)){
		debug_logMessage(PGM_ON, L_INFO, (char *)PSTR("Reading GPS data\r\n"));
	}
	sprintf(packetGPS.sat, "SAT=%d;", GPS.Sats);
	sprintf(packetGPS.lat, "LAT=%.6f;", convertToDecimal(GPS.latitude));
	sprintf(packetGPS.lon, "LON=%.6f;", convertToDecimal(GPS.longitude));
	kernel_addTask(readGPS, 500, PRIORITY_MID);
}