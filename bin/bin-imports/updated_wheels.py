import sys
from encoder import encoder  # encoder.py has encoder class that takes 2 pins for quadrature encoders
import pins
import time
from Adafruit_MCP230xx import *
import RPi.GPIO as io
import wiringpi
from pidController import pidController

# motor numbers: LF=0 RF=1 LB=2 RB=3
# array of motor encoders
encoders = [0 for a in range(4)]
pidControllers = [0 for a in range(4)]


# CALL THIS BEFORE RUNMOTORS
# sets up GPIO, encoders, interrupts
def initMotors():
    wiringpi.wiringPiSetupGpio()  # wiringpi setup
    io.setmode(io.BCM)  # gpio pinmode

    pidControllers[0] = pidController(1, 0, 0)
    pidControllers[1] = pidController(1, 0, 0)
    pidControllers[2] = pidController(1, 0, 0)
    pidControllers[3] = pidController(1, 0, 0)

    for i in range(4):
        encoders[i] = encoder(pins.motorEncoderA[i], pins.motorEncoderB[i])  # set up encoder object
        (pins.mcp20).config(pins.motorEnableA[i], pins.OUTPUT)  # set up the h-bridge a and b pins
        (pins.mcp20).config(pins.motorEnableB[i], pins.OUTPUT)
        # io.setup(pins.motorPWM[i], io.OUT)
        wiringpi.pinMode(pins.motorPWM[i], pins.OUTPUT)  # set up wiringpi software pwms
        error = wiringpi.softPwmCreate(pins.motorPWM[i], 0, 1000)
        if error != 0:
            print "error with wiringpi PWM setup in motortest for motor %s" % i
        stop(i)  # make sure the motor is stopped


# TODO test this make sure it works
# motor numbers: LF=0 RF=1 LB=2 RB=3
# directions: 1 = clockwise, 0 = counter_clockwise
def runMotors(pulses, speed, dirLF, dirRF, dirLB, dirRB):
    # set pulses to 0
    for i in range(4):
        encoders[i].reset()
        pidControllers[i].setSetpoint(speed)

    # start all motors in desired directions
    rotate(0, speed, dirLF)
    rotate(1, speed, dirRF)
    rotate(2, speed, dirLB)
    rotate(3, speed, dirRB)

    # stop when right front motor hits pulse number
    moving = True
    numMotorsRotating = 4
    while numMotorsRotating != 0:
        for i in range(4):
            pidControllers[i].update(encoders[i].getSpeed())
            setSpeed(i, int(pidControllers[i].getOutput()))

        time.sleep(.001)
        print "encoder speeds: 0=%s, 1=%s, 2=%s, 3=%s" % (encoders[0].getSpeed(), encoders[1].getSpeed(), encoders[2].getSpeed(), encoders[3].getSpeed())
        

        for i in range(4):
            if abs(encoders[i].getPulses()) >= abs(pulses):
                stop(i)
                numMotorsRotating = numMotorsRotating - 1
    #for i in range(4):
     #   stop(i)
        #moving = False

    print "final pulses: FL/0=%s, FR/1=%s, BL/2=%s, BR/3=%s" % (
    encoders[0].getPulses(), encoders[1].getPulses(), encoders[2].getPulses(), encoders[3].getPulses())
    # stop when all motors hit pulse number (option 2)
    # ==============================================================
    # numMotorsRotating = 4
    # while numMotorsRotating != 0:
    #     for i in range(4):
    #         if abs(encoders[i].getPulses()) >= abs(pulses):
    #             stop(i)
    #             numMotorsRotating = numMotorsRotating - 1
    # ==============================================================


# helper methods used by runMotors

# motor numbers: LF=0 RF=1 LB=2 RB=3
def rotate(motor_number, speed, dir):
    if dir == 1:
        clockwise(motor_number, speed)
    else:
        counter_clockwise(motor_number, speed)


def clockwise(motor_number, speed):
    setSpeed(motor_number, speed)
    (pins.mcp20).output(pins.motorEnableA[motor_number], pins.HIGH)
    (pins.mcp20).output(pins.motorEnableB[motor_number], pins.LOW)


def counter_clockwise(motor_number, speed):
    setSpeed(motor_number, speed)
    (pins.mcp20).output(pins.motorEnableA[motor_number], pins.LOW)
    (pins.mcp20).output(pins.motorEnableB[motor_number], pins.HIGH)


def stop(motor_number):
    setSpeed(motor_number, 0)
    (pins.mcp20).output(pins.motorEnableA[motor_number], pins.LOW)
    (pins.mcp20).output(pins.motorEnableB[motor_number], pins.LOW)


def setSpeed(motor_number, speed):
    if 0 < speed <= 1000:
        wiringpi.softPwmWrite(pins.motorPWM[motor_number], speed)
    else:
        wiringpi.softPwmWrite(pins.motorPWM[motor_number], 0)


def calibrateMotors():
    speeds = []
    speeds = findSpeeds()
    # TODO math for correct speeds based on relative speeds
    for i in range(4):
        setSpeed(500)


# get the speed of each motor in pulses/second
def findSpeeds():
    for i in range(4):
        encoders[i].reset()
    rotate(0, 500, 1)
    rotate(1, 500, 1)
    rotate(2, 500, 1)
    rotate(3, 500, 1)
    delay(60)
    return [encoders[0].getPulses() / 60, encoders[1].getPulses() / 60, encoders[2].getPulses() / 60,
            encoders[3].getPulses() / 60]

