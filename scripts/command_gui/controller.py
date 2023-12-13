import serial
import serial.tools.list_ports
import threading
import math
import time
from comms.packetID import PacketIDs
from comms.packet import PacketHeader, Packet
from comms.comms_layer import PacketSerializer
from time import sleep

class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.isAppRunning = True
        self._isPortOpen = False

    def close(self):
        self.isAppRunning = False

    def getComPortList(self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(0, "Select an option")
        return comPortList
        
    def openSerialPort(self, port: str, baudrate: str) -> None:
        
        if (self._isPortOpen):
            print("There is already one active connection")
            return
        
        self.view.updateSerialConsole(f"Opening {port} @ {baudrate}")
        self._openPort = serial.Serial()
        self._openPort.baudrate = baudrate
        self._openPort.port = port
        self._openPort.timeout = 1
        self._openPort.xonxoff = 1
        self._openPort.bytesize = serial.EIGHTBITS
        self._openPort.parity = serial.PARITY_NONE
        self._openPort.stopbits = serial.STOPBITS_ONE
        
        try:
            self._openPort.open()
            self._isPortOpen = True
            self._t1 = threading.Thread(target=self.serialUpdate, args=())
            self._t1.start()
        except serial.SerialException as e:
            self._isPortOpen = False
            print(f"Warning: Port is not accessible - {e}")
        
    def serialUpdate(self) -> None:
        # Append items to a buffer. Once a terminating character appears, then I can process and reset the buffer
        # How to handle sending data to the right place? 
        while (self._isPortOpen):
            
            if (self._openPort.in_waiting > 0):
                # Read in all the available bytes
                buffer = self._openPort.read(self._openPort.in_waiting)
                decodedBuffer = buffer.decode('utf-8')
                self.view.updateSerialConsole(decodedBuffer)            
                print(f"{buffer}")        
                # What terminates a line in Python? xx
                # Cases to handle
                # Multiple data lines in the buffer. 
                # Single line
                # Incomplete line 
                

            sleep(0.005)
                            
    def sendString(self, string : str):
        if (self._isPortOpen):
            self._openPort.write((string).encode())
            print("String sent!")
            print(string)

    def closeSerialPort(self, port: str):
        # Must close serial port and the thread
        if (self._isPortOpen):
            self._openPort.close()
            self._isPortOpen = False
            self._t1.join()
        else:
            print(f"Port {port} is already closed")

    def decodeSerial(self):
        pass

    def generateSineWaveDataPoint(self, i):
        x = math.sin(i)
        return (i, x)
    
    def generateCosWaveDataPoint(self, i):
        x = math.cos(i)
        return (i, x)

        

def main():
    from model import Model
    from view import View
    import tkinter as tk
    view = View(tk.Tk())
    model = Model()
    controller = Controller(model, view)
    # controller.sendPacket(PacketIDs.BEGIN)
    # controller.sendPacket(PacketIDs.STANDBY)

if __name__ == "__main__":
    main()