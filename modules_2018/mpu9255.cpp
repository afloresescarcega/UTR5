#include <stdio.h>
#include "mpu9255.h"
#include "io.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

//very simple, just reads from data registers, not accurate yet
//wiringPiI2CWriteReg8(fd, reg, data);
//wiringPiI2CReadReg8(fd, reg);
//negative return is error
//http://www.waveshare.com/wiki/10_DOF_IMU_Sensor_Datasheets


//REGISTERS
#define IMU_I2C_ID			0x68
#define MPU9255_WHO_AM_I	0x75
#define MPU9255_ID			0x73

#define GYRO_CONFIG         0x1b
#define GYRO_LPF            0x1a

#define ACCEL_CONFIG        0x1c
#define ACCEL_LPF           0x1d

#define PWR_MGMT_1          0x6b
#define PWR_MGMT_2          0x6c

//H=high, L=low, high and low bytes of 16 bit data
#define ACCEL_XOUT_H        0x3b
#define ACCEL_XOUT_L        0x3c
#define ACCEL_YOUT_H        0x3d
#define ACCEL_YOUT_L        0x3e
#define ACCEL_ZOUT_H        0x3f
#define ACCEL_ZOUT_L        0x40

#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48


mpu9255::mpu9255() {
	//make sure i2c has been set up (io init function)
	//write to registers to set up (datasheets)

	//make sure the imu is a mpu9255
	int id = imu_i2cRead(1, MPU9255_WHO_AM_I);
	if (id != MPU9255_ID) {
		fprintf(stderr, "error, not the right kind of imu %d\n", id);
	}

	//reset imu
	imu_i2cWrite(1, PWR_MGMT_1, 0x80);
	delay(100);
	imu_i2cWrite(1, PWR_MGMT_1, 0x00);

	//gyro init
	imu_i2cWrite(1, GYRO_CONFIG, 0x00);
	imu_i2cWrite(1, GYRO_LPF, 0x00);		//low pass filter

	//accel init
	imu_i2cWrite(1, ACCEL_CONFIG, 0x00);
	imu_i2cWrite(1, ACCEL_LPF, 0x00);		//low pass filter

	//sample rate register SMPRT_DIV

	//compass init ("external sensor" registers in datasheet)

	//enable accel and gyro
	imu_i2cWrite(1, PWR_MGMT_1, 0x01);
	imu_i2cWrite(1, PWR_MGMT_2, 0x00);

	//fifo set up
	fprintf(stderr, "mpu9255 initialized\n");
}

double mpu9255::accelX() {
	return imu_i2cRead(2, ACCEL_XOUT_H);
}

double mpu9255::accelY() {
	return imu_i2cRead(2, ACCEL_YOUT_H);
}

double mpu9255::accelZ() {
	return imu_i2cRead(2, ACCEL_ZOUT_H);
}



double mpu9255::gyroX() {
	return imu_i2cRead(2, GYRO_XOUT_H);
}

double mpu9255::gyroY() {
	return imu_i2cRead(2, GYRO_YOUT_H);
}

double mpu9255::gyroZ() {
	return imu_i2cRead(2, GYRO_ZOUT_H);
}



double mpu9255::compassX() {
	return 0.0;
}

double mpu9255::compassY() {
	return 0.0;
}

double mpu9255::compassZ() {
	return 0.0;
}



