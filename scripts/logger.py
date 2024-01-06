import serial
import time
import csv
import signal
import sys
from datetime import datetime
import msvcrt

def signal_handler(sig, frame):
    sys.exit(0)

ser = serial.Serial('COM18')
ser.port = 'COM18'
ser.flushInput()
initTime = time.time()
signal.signal(signal.SIGINT, signal_handler)
ser.write("t".encode())
ser.write("z".encode())
isRecordingActive = False
file = 0
while True:
    char = ''
    if msvcrt.kbhit():
        char = msvcrt.getch().decode()
    
    if char == ' ' and isRecordingActive == False:
        isRecordingActive = True
        date = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        file = open(f"../datasets/{date}-pendulum.csv", "a")
        file.write("timestamp (python), timestamp (Teensy), Angular pos (deg), AGC, Status, Angular Vel (deg)\n")
        ser.write("t".encode())
        ser.write("z".encode())
    elif char == ' ' and isRecordingActive == True:
        isRecordingActive = False
    if char == 'q':
        break
    
    ser_bytes = ser.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
    buffer = f"{time.time() - initTime}, {decoded_bytes}\n"
    if isRecordingActive:
        file.write(buffer)
        print("R: " + decoded_bytes)
    else:
        print(decoded_bytes)
    

file.close()