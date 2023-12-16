import serial
import serial.tools.list_ports
import threading
import math
import time
from comms.packetID import PacketIDs
from comms.packet import PacketHeader, Packet
from comms.comms_layer import PacketSerializer
from time import sleep
import commands 

class Controller:
    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.isAppRunning = True
        self._isPortOpen = False
        self.initCliCommandFunctions()
        
    def initCliCommandFunctions(self):
        self.cliCommandFunctions = {}
        self.cliCommandFunctions["Begin"] = self.cliCommandBeginActivation
        self.cliCommandFunctions["Standby"] = self.cliCommandStandbyActivation
        self.cliCommandFunctions["Calibrate"] = self.cliCommandCalibrateActivation

    def close(self):
        self.isAppRunning = False
        self.closeSerialPort()

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
            successStr = f"{port} is now open"
            print(successStr)
            self.view.updateSerialConsole(successStr)
        except serial.SerialException as e:
            self._isPortOpen = False
            errorStr = f"Warning: {port} is not accessible - {e}"
            print(errorStr)
            self.view.updateSerialConsole(errorStr)
        
    def serialUpdate(self) -> None:
        # Append items to a buffer. Once a terminating character appears, then I can process and reset the buffer
        # How to handle sending data to the right place? 
        buffer = []
        while (self._isPortOpen):
            
            while (self._openPort.in_waiting > 0):
                character = self._openPort.read(1)
                buffer.append(character)

            # Find first instance of FRAMING_START
            


            # Read in all the available bytes
            # buffer = self._openPort.read(self._openPort.in_waiting)
            
            decodedBuffer = buffer.decode('utf-8')
            self.view.updateSerialConsole(decodedBuffer)            
            print(f"{buffer}")
                
                # For now assume we get one line per msg

                # Check for mode activation 
                #   - Mode activation consists of a command followed by the "OK" string


                # What terminates a line in Python? xx
                # Cases to handle
                # Multiple data lines in the buffer. 
                # Single line
                # Incomplete line 
    def unpackFrame(self, buffer : bytearray) -> str:
        # Find first instance of FRAMING_START, and first instance of FRAMING_END
        buffer.find(commands.FRAMING_START.to_bytes, )


    def checkForModeActivation(self, buffer : bytearray) -> None:
        # Easiest to process with a decoded array
        decodedBuffer = buffer.decode()
        
        # Find substring ok. Return if not OK
        isStringOk = decodedBuffer.find('OK')
        if (isStringOk == -1):
            return
        
        # Find command between first index and first space. 
        firstSpaceIndex = decodedBuffer.find(' ')
        if (firstSpaceIndex == -1):
            print("Malformed command")
            return
        
        # Check command matches the pre-defined commands
        cliCommandSubstring = decodedBuffer[0:firstSpaceIndex]
        for command in commands.commands:
            if command == cliCommandSubstring:
                self.cliCommandFunctions[command]()
                # Do something
                # Call the respective activation function
                # E.g. 
                # "cli - OK" will never be sent. 
                # "Begin - OK" will change a switch such that all streamed data will now go to the 
                # same place. 
                # "Standby - OK" will switch off the data stream
                # "Calibration - OK" will stream data to calibration part of the model
                # "Reset-IMU - OK" will do nothing
                # "Motor - OK" Will probably do nothing. But I could send motor data across the comm
                # "Help" will do nothing. (Unlikely to be handled by this function)
                pass

    def handleCommand():
        pass

    # Process the buffer and display it on the terminal. I.e, break up string based on newlines
    def displayInTerminal(buffer : bytearray):
        pass

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

    def cliCommandBeginActivation(self):
        pass

    def cliCommandStandbyActivation(self):
        pass

    def cliCommandCalibrateActivation(self):
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