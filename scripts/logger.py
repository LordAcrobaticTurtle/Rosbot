import serial
import time
import csv
import signal
import sys


def signal_handler(sig, frame):
    sys.exit(0)

ser = serial.Serial('COM18')
ser.port = 'COM18'
ser.flushInput()
initTime = time.time()
signal.signal(signal.SIGINT, signal_handler)

while True:
    try:
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        print(decoded_bytes)
        with open("test_data.csv","a") as f:
            writer = csv.writer(f,delimiter=",")
            writer.writerow([time.time()-initTime,decoded_bytes])
    except Exception as e:
        print(f"keyboard interrupt: {e}")
        break