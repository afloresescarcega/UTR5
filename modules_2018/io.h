#ifndef IO
#define IO

#define OUT 0
#define IN 1
#define PWM 2

bool initGPIO();
bool pinSetup(int pin, int setting);
void digitalOut(int pin, int value);
void analogOut(int pin, int value);
int digitalIn(int pin);

int imu_i2cRead(int bytes, int reg);
void imu_i2cWrite(int bytes, int reg, int data);
#endif