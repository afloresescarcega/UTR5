#include <stdio.h>
#include "io.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>

//io file for pi
//these functions are to simplify code between microcontrollers
//we need an arduino version of this

//when you compile this code on a raspberry pi, link wiringPi and softPwm
//make sure i2c is enabled
//g++ -o main Main.cpp hbridge.cpp io.cpp mpu9255.cpp -l wiringPi -l pthread

#define IMU_I2C_ID			0x68
int imu_fd;		//id for wiringpi i2c


bool initGPIO() {
	//make sure that wiringpi setup is only called once
	#ifndef WIRINGPIINIT
	#define WIRINGPIINIT
	wiringPiSetupGpio();
	imu_fd = wiringPiI2CSetup(IMU_I2C_ID);
	if (imu_fd < 0) {
		fprintf(stderr, "error initializing i2c for mpu9255\n");
	}
	#endif
}

/*
 * setting eithr OUT, IN, PWM
 */
bool pinSetup(int pin, int setting) {
	if (setting == OUT) {
		pinMode(pin, OUTPUT);
		return true;
	}
	else if (setting == IN) {
		pinMode(pin, INPUT);
		return true;
	}
	else if (setting == PWM) {
		if (softPwmCreate(pin, 0, 1000) == 0) {
			return true;
		}
	}
	else {
		return false;
	}
}

void digitalOut(int pin, int value) {
	digitalWrite(pin, value);
}

void analogOut(int pin, int value) {
	softPwmWrite(pin, value);
}

int digitalIn(int pin) {
	return digitalRead(pin);
}

int imu_i2cRead(int bytes, int reg) {
	if (bytes == 1) {
		return wiringPiI2CReadReg8(imu_fd, reg);
	}
	else if (bytes == 2) {
		return wiringPiI2CReadReg16(imu_fd, reg);
	}
}

void imu_i2cWrite(int bytes, int reg, int data) {
	if (bytes == 1) {
		wiringPiI2CWriteReg8(imu_fd, reg, data);
	}
	else if (bytes == 2) {
		wiringPiI2CWriteReg16(imu_fd, reg, data);
	}
}
