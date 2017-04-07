#include <stdio.h>
#include <vector>
#include <wiringPi.h>
#include "hbridge.h"
#include "io.h"

//for other controllers replace pins
//when you compile this code on a raspberry pi, make sure wiringpi is installed
//to compile: go to directory, compile and link wiringPi and softPwm
//	g++ -o main Main.cpp hbridge.cpp io.cpp -l wiringPi -l pthread
//to run: ./main

//pin definitions
int motorEnableA[] = { 5, 13 };
int motorEnableB[] = { 6, 19 };
int motorPWM[] = { 20, 21 };

int main(int argc, char*argv[]) {
	initGPIO();


	//create a list of 2 motors
	std::vector<HBridgeMotor> motors;
	for (int i = 0; i < 2; i++) {
		motors.push_back(HBridgeMotor(motorEnableA[i], motorEnableB[i], motorPWM[i]));
	}

	//test one motor
	motors[0].setSpeed(700);
	motors[0].rotate(1);
	for (int i = 0; i < 100000000; i++) {}		//delay
	
	motors[0].setSpeed(200);
	motors[0].rotate(0);
	for (int i = 0; i < 100000000; i++) {}		//delay

	motors[0].stop();
	fprintf(stderr, "motors stopped\n");
}
