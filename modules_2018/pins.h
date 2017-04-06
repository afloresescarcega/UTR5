#ifndef PINS
#define PINS

#define OUT 0
#define IN 1
#define PWM 2

int motorEnableA[] = { 5, 13 };
int motorEnableB[] = { 6, 19 };
int motorPWM[] = { 20, 21 };

bool initGPIO();
bool pinSetup(int pin, int setting);
void digitalOut(int pin, int value);
void analogOut(int pin, int value);
int digitalIn(int pin);


#endif