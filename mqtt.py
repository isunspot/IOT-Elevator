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

def readNumber():
    number = bus.read_byte_data(address,1)
    return number

def on_connect(client , userdata , flags , rc ):
    print ("connected to broker ")
    client.subscribe("mqtt2") 

def on_message(client , userdata , msg):
    if(str(msg.payload) == "floor0"):
        print("Floor 0 pressed")
        writeNumber(0)  
    if(str(msg.payload) == "floor1"):
        print("Floor 1 pressed")
        writeNumber(1)
    if(str(msg.payload) == "floor2"):
        print("Floor 2 pressed")
        writeNumber(2)
    if(str(msg.payload) == "floor3"):
        print("Floor 3 pressed")
        writeNumber(3)

        
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost" , 1883 , 60 )


client.loop_forever()
    
    
