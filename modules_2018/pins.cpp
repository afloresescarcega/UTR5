#include <stdio.h>
#include "pins.h"
#include <wiringPi.h>
#include <softPwm.h>

//these functions are to simplify code while we decide microcontroller
//if we want to, we can use all code on another controller by replacing this cpp file and using the same header file

//when you compile this code on a raspberry pi, link wiringPi and softPwm
//g++ -o main Main.cpp hbridge.cpp pins.cpp -l wiringPi -l pthread




bool initGPIO() {
	//make sure that wiringpi setup is only called once
	#ifndef WIRINGPIINIT
	#define WIRINGPIINIT
	wiringPiSetupGpio();
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