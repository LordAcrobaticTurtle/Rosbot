import serial
import serial.tools.list_ports
import threading
import math
import time
from comms.packetID import PacketIDs
from comms.packet import PacketHeader
from comms.comms_layer import PacketSerializer

class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.isAppRunning = True

    def getComPortList(self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(0, "Select an option")
        return comPortList
        
    def openSerialPort(self, port: str, baudrate: str) -> None:
        self.view.updateSerialConsole(f"Opening {port} @ {baudrate}")
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

        # threading.Thread(target=self.handleSerialOpen, args=(), daemon=1)

    # def handleSerialOpen(self, port: str, baudrate: str) -> None:
        

    def serialUpdate(self) -> None:
        
        while (self.isAppRunning):
            
            if (self._openPort.in_waiting > 0):
                # Read in all the available bytes
                buffer = self._openPort.read(self._openPort.in_waiting)

                # Find the first instance of a packet in the byte stream. Discard all other packets
                # What I would prefer is to find all packets in a byte stream 
                # Incomplete frames in the buffer will be dropped
                startpos = PacketSerializer.findIdentifyingByte(buffer)
                header = PacketSerializer.decodeHeader(buffer, startpos)
                data = PacketSerializer.decodeData(buffer, header, startpos)
                
                # Now do something with the data
                self.handlePacket(header, data)

                
    def sendPacket(self, packetID: PacketIDs):
        if (self.isAppRunning and self._openPort.is_open):
            

    def handlePacket(self, header : PacketHeader, data):
    
        if header.packetID == PacketIDs.BEGIN:
            pass
        elif header.packetID == PacketIDs.STANDBY:
            pass
        elif header.packetID == PacketIDs.ESTOP:
            pass
        elif header.packetID == PacketIDs.REQUEST:
            pass
        elif header.packetID == PacketIDs.STATE:
            # Store into the model and update view
            pass
        

    def closeSerialPort(self, port: str):
        self._openPort.close()


    def decodeSerial(self):
        pass


    def generateSineWaveDataPoint(self, i):
        x = math.sin(i)
        return (i, x)
    
    def generateCosWaveDataPoint(self, i):
        x = math.cos(i)
        return (i, x)

        