import serial
import serial.tools.list_ports
import threading
import math
import time
        

class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view

    def getComPortList(self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(0, "Select an option")
        return comPortList
        
    def openSerialPort(self, port: str, baudrate: str) -> None:
        threading.Thread(target=self.handleSerialOpen, args=(), daemon=1)

    def handleSerialOpen(self, port: str, baudrate: str) -> None:
        self._openPort = serial.Serial()
        self._openPort.baudrate = baudrate
        self._openPort.port = port
        self._openPort.timeout = 1
        self._openPort.xonxoff = 1
        self._openPort.bytesize = serial.EIGHTBITS
        self._openPort.parity = serial.PARITY_NONE
        self._openPort.stopbits = serial.STOPBITS_ONE
        self._openPort.open()
        self._isPortOpen = True
        self._t1 = threading.Thread(target=self.serialUpdate, args=(), daemon=1)
        self._t1.start()


    def serialUpdate(self) -> None:
        print("Serial update")
        pass

    def closeSerialPort(self, port: str):
        self._openPort.close()


    def decodeSerial(self):
        # Writing bytes into the buffer. When do I start vs stop reading
        pass


    def generateSineWaveDataPoint(self, i):
        x = math.sin(i)
        return (i, x)
    
    def generateCosWaveDataPoint(self, i):
        x = math.cos(i)
        return (i, x)

        