#!/usr/bin/env python3

# Requirements
# paho-mqtt
# serial

import serial
import paho.mqtt.client as mqtt
import time
import threading
import logging

# init logger
logger = logging.getLogger("projector_logger")
logger.setLevel(logging.DEBUG)

fh = logging.FileHandler("/home/projectorcontrol/Dokumente/proj3.log")
fh.setLevel(logging.DEBUG)
formatter = logging.Formatter("%(asctime)s - %(levelname)s -- %(message)s", datefmt="%d-%m-%Y %H:%M:%S")
fh.setFormatter(formatter)
logger.addHandler(fh)


# init MQTT
broker = "PLACEHOLDER"
port = 1883
username = "PLACEHOLDER"
password = "PLACEHOLDER"
topic = "projector/3/in/status"
topics = [("projector/3/out/power", 0), ("projector/3/out/status", 0)]
client_id = "projector3"
count = 0

# init SERIAL
ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 115200

ON     = bytearray([0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x00,0x00,0x5D])
OFF    = bytearray([0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x01,0x00,0x5E])
STATUS = bytearray([0x07,0x14,0x00,0x05,0x00,0x34,0x00,0x00,0x11,0x00,0x5E])

# ----- status CHECK ----- #
    
def check_status():
    if not ser.is_open:
        ser.open()
    #print("Port is open:", ser.is_open)
    
    ser.write(STATUS) # b'\x03\x14\x00\x00\x00\x14\x05\x14\x00'
    status = ser.read(9)
    print("STATUS", status)
    # print("on", status == b'\x05\x14\x00\x03\x00\x00\x00\x01\x18')
    # print("off", status == b'\x05\x14\x00\x03\x00\x00\x00\x00\x17')
    
    ser.close()
    #print("Port is open:", ser.is_open)
    #print("")
    
    send_status(status)

 # send mqtt message of status to ioBroker
def send_status(status): 
    mqtt_status = ""
    if status == b'\x03\x14\x00\x00\x00\x14\x05\x14\x00' or status == b'\x05\x14\x00\x03\x00\x00\x00\x01\x18':
        mqtt_status = "true"
    elif status == b'\x05\x14\x00\x03\x00\x00\x00\x00\x17':
        mqtt_status = "false"
        
    publish(mqtt_status)

def publish(msg):
    global client
    result = client.publish(topic, msg) 
    status = result[0]
    if status == 0:
        print(f"Send `{msg}` to topic `{topic}`")
#        logger.info(f"Send `{msg}` to topic `{topic}`")
    else:
        print(f"Failed to send message to topic {topic}")
        logger.warning(f"Failed to send message ({msg}) to topic ({topic})")

# ----- SERIAL control ----- #

def control_projector(on):

    if not ser.is_open:
        print("opening port...")
        ser.open()    
        
    # send command (on/off)
    status = ""
    if on == "true":
        print(">> turning on")
        logger.info("turning projector on")
        ser.write(ON)
        
        print("sleep...")
        time.sleep(30)  
        
#        print("STATUS:")
 #       ser.write(STATUS) 
  #      status = ser.read(9) # b'\x03\x14\x00\x00\x00\x14\x05\x14\x00'
   #     print(status)

    else:
        print(">> turning off")
        logger.info("turning projector off")
        ser.write(OFF)
        
        print("sleep...")
        time.sleep(30)
        
#        print("STATUS:")
 #       ser.write(STATUS) 
  #      status = ser.read(9) # b'\x05\x14\x00\x03\x00\x00\x00\x00\x17'
   #     print(status)
    
    # close serial
    #print("closing...")
    ser.close()
    #print("Port is open:", ser.is_open)
    print("")
    
    check_status()

prev = None
def execute_command(command):
    global prev
    # don't execute the same commands (if they are received via
    # doesn't seem to really matter that much
    if prev == command:
        print("[SAME] command --> return\n")
        return
    
    # check (previous) status of projector:
        # if on --> only turn off
    
    print("executing...")
    control_projector(command)
    # NOTE: if projector is not done booting, executing a command won't do anything --> might missalign ioBroker
        # send status via mqtt?
        # block turn on/off until completely done?
    
    prev = command
    

# ----- MQTT control ----- #

def disconnect(client):
    client.disconnect()
    client.loop_stop()
    print("disconnected")
    logger.warning("MQTT Broker disconnected")  
    

def on_connect(client, userdata, flags, rc):
    print("connecting...")
    if rc == 0:
        print("Connected to MQTT Broker")
#        logger.info("Connected to MQTT Broker")
        client.subscribe(topics)
    else:
        print("Failed to connect, return code %d\n", rc)
        logger.warning(f"Failed to connect to MQTT Broker ({rc})")
        
def on_message(client, userdata, msg):
    global count
    command = msg.payload.decode()
    print(f"got message_{count}: {msg.topic} = {command}")
#    logger.info(f"got message_{count}: {msg.topic} = {command}")
    
    if msg.topic == "projector/3/out/status":
        if command == "true":
            print("check status...")
            check_status()
    elif msg.topic == "projector/3/out/power":
        execute_command(command)

    # for test
    count += 1
    #if count == 20:
        #disconnect(client)

def init_client():
    try:
        # client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
        client = mqtt.Client()
    except:
        print("Error1")
        logger.error("couldn't init client")
        return
    client.username_pw_set(username,password)
    client.on_connect = on_connect
    client.on_message = on_message
    try:
        client.connect(broker, port)
        print("connected")
        return client
    except:
        print("Error")
        logger.error("Error on connection")
    return client

logger.info("starting script")
client = init_client()
client.loop_forever(retry_first_connection=True) # should reconnect automatically if connection is broken





