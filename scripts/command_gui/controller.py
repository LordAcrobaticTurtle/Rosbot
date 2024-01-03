import serial
import serial.tools.list_ports
import threading
import math
import time
from datetime import datetime
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
        self._isMockPortOpen = False
        self._isRecordingActive = False

    def close(self):
        self.isAppRunning = False
        self.closeSerialPort()
        self.closeMockSerialPort()

    def getComPortList(self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(0, "Select an option")
        comPortList.insert(len(comPortList), "Mock Connection")
        return comPortList
        
    def openMockSerialPort(self, port : str, baudrate : str) -> None:
        if (self._isPortOpen):
            print("There is already one active connection")
            return
        self.view.updateSerialConsole(f"Opening {port} @ {baudrate}")
        self._isMockPortOpen = True
        self._t1 = threading.Thread(target=self.updateMockSerialPort, args=())
        self._t1.start()
        
    def closeMockSerialPort(self) -> None:
        if (self._isMockPortOpen):
            # Close it
            self._isMockPortOpen = False
            self._t1.join()
            self.view.updateSerialConsole(f"Mock port has been closed")
        
    def updateMockSerialPort(self) -> None:
        print("Mock Start")
        timestamp = 0.0
        timestep = 0.1
        while (self._isMockPortOpen):
            # print("Mock update")
            timestamp += timestep
            packet = commands.LocalisationPacket()
            sinX = self.generateSineWaveDataPoint(timestamp)
            cosX = self.generateCosWaveDataPoint(timestamp)
            buffer = f"({sinX[1]},{cosX[1]},{sinX[1] + cosX[1]}),({sinX[1]},{cosX[1]},{sinX[1] + cosX[1]}),({sinX[1]},{cosX[1]},{sinX[1] + cosX[1]}),(9.1,8.9)"
            packet.fromString(buffer)
            self.model.insertCalibrationPacket(packet, timestamp)
            time.sleep(timestep)


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
        buffer = str()
        while (self._isPortOpen):
            while (self._openPort.in_waiting > 0):
                character = self._openPort.read(1)
                buffer += character.decode()
            
            # Unpack frame from 0x2 0x3 delimiters
            unpackedBuffer = self.unpackFrame(buffer)

            if (unpackedBuffer is not None):
                self.processFrame(unpackedBuffer)
                # Reset buffer
                buffer = str()
            
    def unpackFrame(self, buffer : str) -> str:
        # Find first instance of FRAMING_START, and first instance of FRAMING_END
        
        frameStart = buffer.find(commands.FRAMING_START)
        if (frameStart == -1):
            # print("No frame start")
            return None
        
        frameEnd = buffer.find(commands.FRAMING_END, frameStart, len(buffer))

        if (frameEnd == -1):
            # print("No frame end")
            return None
        
        unpackedBuffer = buffer[frameStart+len(commands.FRAMING_START)+1:frameEnd]
        return unpackedBuffer
        # Print data in between for now

    def processFrame(self, unpackedBuffer : str): 
        # print(unpackedBuffer)
        # There is a frame to process. 
        # Decipher and print arguments
        # print(unpackedBuffer.split(' '))
        splitBuffer = unpackedBuffer.split(' ')
        commandIndexFromBuffer = int(splitBuffer[0])
        timestamp = int(splitBuffer[1])
        data = splitBuffer[2]
        serialStr = f"CommandIndex: {commandIndexFromBuffer}, t: {timestamp}, d: {data}"
        
        self.view.updateSerialConsole(serialStr)
        # What to do with different information
        # Begin, standby, Calibrate, reset-IMU, Motor, Help -> Send response to terminal
        if (commandIndexFromBuffer >= commands.CliCommandIndex.CLI_BEGIN and 
            commandIndexFromBuffer <= commands.CliCommandIndex.CLI_HELP):
            # Update
            # parse for newlines and updates serial console appropriately
            # Create objects to retrieve the data from
            pass
        elif (commandIndexFromBuffer == commands.CliCommandIndex.CLI_CONTROL_PACKET):
            
            # Parse and store in control packet location
            pass
        elif (commandIndexFromBuffer == commands.CliCommandIndex.CLI_LOCALISATION_PACKET):
            # Parse and store in localisation packet location
            packet = commands.LocalisationPacket()
            packet.fromString(data)
            self.model.insertCalibrationPacket(packet, timestamp)
        

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


    def sendString(self, string : str):
        if (self._isPortOpen):
            self._openPort.write((string).encode())
            print("String sent!")
            print(string)

    def closeSerialPort(self):
        # Must close serial port and the thread
        if (self._isPortOpen == True):
            self._openPort.close()
            self._isPortOpen = False
            self._t1.join()
            print(f"Port has been closed")

    def activateRecording(self, directoryPath : str):
        self._isRecordingActive = True
        # Open the file specified by the filepath in the main window
        self.openCalibrationFile(directoryPath)
        # Register callback function
        funcAndId = {
            "id":"PacketRecording",
            "func" : self.newCalibPacketCb
        }
        self.model.registerCalibrationCallbackFunction(funcAndId)

    def openCalibrationFile(self, directoryPath : str):

        if directoryPath == None:
            directoryPath = "B:/Projects/Rosbot/Data"
        currTime = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        self._calibrationFile = open(directoryPath + "/" + currTime + ".csv", "w")
        self._calibrationFile.write("timestamp, a_x, a_y, a_z, gr_x, gr_y, gr_z, ga_x, ga_y, ga_z, vel L, vel R, pendAngle\n")

    def deactivateRecording(self):
        self._isRecordingActive = False
        self.model.deregisterCalibrationCallbackFunction("PacketRecording")
        self._calibrationFile.close()
        
    def newCalibPacketCb(self, timestamp : float, data : commands.LocalisationPacket):
        if (self._isRecordingActive == False):
            return
        buffer = ""
        buffer += f"{timestamp}, "
        buffer += f"{data.accel.x}, {data.accel.y}, {data.accel.z}, "
        buffer += f"{data.gyroRates.x}, {data.gyroRates.y}, {data.gyroRates.z}, "
        buffer += f"{data.orientation.x}, {data.orientation.y}, {data.orientation.z}, "
        buffer += f"{data.vwheel.v1}, {data.vwheel.v2}, {data.pendulumAngle}\n"
        self._calibrationFile.write(buffer)
        print("Cb: " + buffer)
        
        

    def generateSineWaveDataPoint(self, i):
        x = math.sin(i)
        return (i, x)
    
    # Returns a tuple for x AND y data
    def generateCosWaveDataPoint(self, i):
        x = math.cos(i)
        return (i, x)

    def getCalibrationPackets(self) -> list:
        return self.model.getCalibrationPackets()

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