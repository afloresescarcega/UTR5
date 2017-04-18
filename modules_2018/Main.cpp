#include <stdio.h>
#include <vector>
#include <wiringPi.h>
#include "hbridge.h"
#include "mpu9255.h"
#include "io.h"

/*pi instructions: (for other controllers replace io file)
 *when you compile this code on a raspberry pi, make sure wiringpi is installed
 *make sure i2c-tools is installed, make sure imu is on i2c 0x68, enable i2c in raspi-config
 *to compile: go to directory, compile and link wiringPi and softPwm (pthread is for soft pwm)
 *	g++ -o main Main.cpp hbridge.cpp io.cpp mpu9255.cpp -l wiringPi -l pthread
 *to run: ./main
 */

void motorTest() {
	//pins {motor1, motor2}
	int motorEnableA[] = { 5, 13 };
	int motorEnableB[] = { 6, 19 };
	int motorPWM[] = { 20, 21 };

	//create a list of 2 motors
	std::vector<HBridgeMotor> motors;
	for (int i = 0; i < 2; i++) {
		motors.push_back(HBridgeMotor(motorEnableA[i], motorEnableB[i], motorPWM[i]));
	}

	//test one motor
	motors[0].setSpeed(700);
	motors[0].rotate(1);

	motors[1].setSpeed(200);
	motors[1].rotate(0);
	
	for (int i = 0; i < 100000000; i++) {}		//delay

	motors[0].stop();
	motors[1].stop();
	fprintf(stderr, "motors stopped\n");
}

void imuTest() {
	mpu9255 imu = mpu9255();
	while (true) {
		int gyroX = imu.gyroX();
		int gyroY = imu.gyroY();
		int gyroZ = imu.gyroZ();
		int accelX = imu.accelX();
		int accelY = imu.accelY();
		int accelZ = imu.accelZ();

		printf("gyro: x%d y%d z%d \t| accel: x%d y%d z%d\n", gyroX, gyroY, gyroZ, accelX, accelY, accelZ);
		delay(100);
	}
}

int main(int argc, char*argv[]) {
	initGPIO();
	//motorTest();
	imuTest();
}
