#ifndef MPU9255
#define MPU9255

class mpu9255 {
private:
	int fd;

public:
	mpu9255();

	double gyroX();
	double gyroY();
	double gyroZ();

	double accelX();
	double accelY();
	double accelZ();

	double compassX();
	double compassY();
	double compassZ();
};


#endif
