import smbus
import time
import RPi.GPIO as GPIO
import paho.mqtt.client as mqtt
GPIO.setmode(GPIO.BCM)
bus = smbus.SMBus(1)


# address setup in the arduino program
address = 0x04

def writeNumber(value):
    bus.write_byte(address,value)
    return -1
#sent to initialise the elevator
writeNumber(5)

def readNumber():
    number = bus.read_byte_data(address,1)
    return number
#declartion of variables for each floor and assigning them GPIO pins
floor0 = 17
floor1 = 23
floor2 = 24
floor3 = 25
floor0_in =5
floor1_in =6
floor2_in =13
floor3_in =19
stop = 26
stoprelease = 21
#setup of GPIO pins
GPIO.setup(floor0, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor1, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor2, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor3, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

GPIO.setup(floor0_in, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor1_in, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor2_in, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(floor3_in, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(stop, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(stoprelease, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

emergency = False
while True:
    s= GPIO.input(stop)
    if(s):        
        print("Emergency button pressed")
        writeNumber(4)
        emergency = not emergency
        #time.sleep(1)
    if (emergency == False):
        # Outside
        num0 = GPIO.input(floor0)
        if(num0):
            print("Floor 0 pressed")
            writeNumber(0)
            
        num1 = GPIO.input(floor1)
        if(num1):
            print("Floor 1 pressed")
            writeNumber(1)
            
        num2 = GPIO.input(floor2)
        if(num2):
            print("Floor 2 pressed")
            writeNumber(2)
            
        num3 = GPIO.input(floor3)
        if(num3):
            print("Floor 3 pressed")
            writeNumber(3)


        # Inside
        num0_in = GPIO.input(floor0_in)
        if(num0_in):
            print("Floor 0 pressed")
            writeNumber(0)
            
        num1_in = GPIO.input(floor1_in)
        if(num1_in):
            print("Floor 1 pressed")
            writeNumber(1)
            
        num2_in = GPIO.input(floor2_in)
        if(num2_in):
            print("Floor 2 pressed")
            writeNumber(2)
            
        num3_in = GPIO.input(floor3_in)
        if(num3_in):
            print("Floor 3 pressed")
            writeNumber(3)
            
    
    
