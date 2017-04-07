#include <stdio.h>
#include "hbridge.h"
#include "pins.h"

#define LOW 0
#define HIGH 1

HBridgeMotor::HBridgeMotor(int motorEnableAPin, int motorEnableBPin, int motorPWMPin) {
	motorEnableA = motorEnableAPin;
	motorEnableB = motorEnableBPin;
	motorPWM = motorPWMPin;

	for (int i = 0; i < 2; i++) {
		pinSetup(motorEnableA, OUT);
		pinSetup(motorEnableB, OUT);
		pinSetup(motorPWM, PWM);
	}

	fprintf(stderr, "h-bridge motor gpio initialized\n");
}

void HBridgeMotor::setSpeed(int speed) {
	if (0 < speed <= 1000) {
		//digitalOut(motorPWM, HIGH);
		analogOut(motorPWM, speed);
	}
	else {
		//digitalOut(motorPWM, LOW);
		analogOut(motorPWM, 0);
	}
}

void HBridgeMotor::clockwise() {
	digitalOut(motorEnableA, HIGH);
	digitalOut(motorEnableB, LOW);
}

void HBridgeMotor::counter_clockwise() {
	digitalOut(motorEnableA, LOW);
	digitalOut(motorEnableB, HIGH);
}

void HBridgeMotor::rotate(int dir) {
	if (dir == 1) {
		clockwise();
	}
	else {
		counter_clockwise();
	}
}

void HBridgeMotor::stop() {
	setSpeed(0);
	digitalOut(motorEnableA, LOW);
	digitalOut(motorEnableB, LOW);
}
