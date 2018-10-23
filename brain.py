import asyncio
import websockets
import signal
import sys
import json

import RPi.GPIO as GPIO
import time

max = 15
min = 7

loop = asyncio.get_event_loop()
uri = 'ws://10.240.228.30:8083' ##This is Michael Newton's and needs to be updated to work properly
messages = []
connections = []
speed = 0

##GPIO.PWM(channel,frequency)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(12, GPIO.OUT)  ##acceleration
GPIO.setup(9,GPIO.OUT)    ##steering
s=GPIO.PWM(9,120)
p = GPIO.PWM(12, 110)

###   Section for message processing ######################
#################################################################
async def handleMessages():
	async with websockets.connect(uri) as socket:
		global messages
		global connections
		global speed
		connections.append(socket)
		while True:
			message = await socket.recv()
			print(message)
			messages.append(message)
			parsed = json.loads(message)
			if parsed['type'] == 'SET_SPEED':
				speed = parsed['payload']
            else if parsed['type']=='SET_STEERING':
                steering = parsed['payload']
#####################################################################


###   Section for setting speed ##################################
###############################################################################
def speedToServo(speed):
	diff = max - min
	return (speed * diff) + min

async def setSpeed():
	global messages
	p.start(1)
	length = 0
	while True:
		print("speed to servo", speedToServo(speed))
		p.ChangeDutyCycle(speedToServo(speed))
		print("speed: ", speed)
		if len(messages) > length:
			for i in range(length, len(messages)):
				print("recieved: " + messages[i])
				length = len(messages)
		await asyncio.sleep(.1)
###############################################################################


### Section for controlling steering #######################
#########################################################################
async def weightingFactor(visionmessage, sensormessage):
    #placeholder for calulcations
    finalvalue = visionmessage*f1 + sensormessage*f2

def steering(finalvalue):

async def setSteering():
    global messages
    s.start(1)
    length=0
    while True:
        print("angle to steering", steering(finalvalue)
        s.ChangeDutyCycle(steering(finalvalue))
        print("steering angle ", steering)
        if len(messages) > length:
            for j in range(length, len(messages)):
                print("recieved: " + messages[j])
                length=len(messages)
        await asyncio.sleep(.1)
###########################################################################


### Section for handling signals #############################
#########################################################################
def signal_handler(signal, frame):
    p.stop()
    s.stop()
    loop.stop()
    GPIO.cleanup()
    for con in connections:
    	con.close()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)
#########################################################################


### keeps the program running #############################
######################################################################
asyncio.ensure_future(handleMessages())
asyncio.ensure_future(doesStuff())
loop.run_forever()
######################################################################






### This is a rough example of what Michael Newton had in mind for individual teams ####
########################################################################
async def sendSerialMessage(port,message):
async def sendSocketMessage(message):

##The team can put whatever code they want here

async def onSocketMessage(message):
	await sendSerialMessage("port1", message)
async def onSerialMessage(port,message):
	await sendSocketMessage(message)
