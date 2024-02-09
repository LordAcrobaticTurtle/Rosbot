# This can be used to open a new thread and poll 
import serial 
import serial.tools.list_ports
import threading
import ui_elements.serial_console as serialView
import commands
import model

class SerialPortHandler():
    def __init__(self, view : serialView.SerialConsole, model : model.Model) -> None:
        self.isAppRunning = True
        self._serialPortLock = threading.Lock()
        self._isPortOpen = False
        self._view = view
        self._model = model

    def populate_callbacks(self) -> None:
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Connect"] = self.open
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Disconnect"] = self.close
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Get List"] = self.get_list_of_available_ports
        
        pass

    def close(self):
        self.isAppRunning = False
        if (self._isPortOpen):
            self._t1.join()
            self._isPortOpen = False

    
    def open(self, port : str, baudrate : str):
        with self._serialPortLock:
            if (self._isPortOpen):
                print("There is already one active connection")
                return

            # self.view.updateSerialConsole(f"Opening {port} @ {baudrate}")

            self._openPort = serial.Serial()
            self._openPort.baudrate = int(baudrate)
            self._openPort.port = port
            self._openPort.timeout = 1
            self._openPort.xonxoff = 1
            self._openPort.bytesize = serial.EIGHTBITS
            self._openPort.parity = serial.PARITY_NONE
            self._openPort.stopbits = serial.STOPBITS_ONE
            
            try:
                self._openPort.open()
                self._isPortOpen = True
                self._t1 = threading.Thread(target=self.threaded_update, args=())
                self._t1.start()
                successStr = f"{port} is now open"
                print(successStr)
                # self.view.updateSerialConsole(successStr)
            except serial.SerialException as e:
                self._isPortOpen = False
                errorStr = f"Warning: {port} is not accessible - {e}"
                print(errorStr)
                # self.view.updateSerialConsole(errorStr)

    def get_list_of_available_ports (self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(0, "Select an option")
        comPortList.insert(len(comPortList), "Mock Connection")
        return comPortList

    def threaded_update(self) -> None:
        buffer = str()
        while (self._isPortOpen):
            
            self._serialPortLock.acquire()
            while (self._openPort.in_waiting > 0):
                character = self._openPort.read(1)
                try: 
                    buffer += character.decode()
                except:
                    print(f"Error attempting to decode: {buffer}")
            
            self._serialPortLock.release()

            # Unpack frame from 0x2 0x3 delimiters
            unpackedBuffer = self.unpackFrame(buffer)

            if (unpackedBuffer is not None):
                self.processFrame(unpackedBuffer)
                # Reset buffer
                buffer = str()

    def send(self, string : str) -> None:
        if (self._isPortOpen):
            self._serialPortLock.acquire()
            self._openPort.write((string + "\n").encode())
            self._serialPortLock.release()
            print("String sent!")
            print(string)


    def unpackFrame(self, buffer : str) -> str:
        # Find first instance of FRAMING_START, and first instance of FRAMING_END
        
        frameStart = buffer.find(commands.FRAMING_START)
        if (frameStart == -1):
            return None
        
        frameEnd = buffer.find(commands.FRAMING_END, frameStart, len(buffer))

        if (frameEnd == -1):
            return None
        
        unpackedBuffer = buffer[frameStart+len(commands.FRAMING_START)+1:frameEnd]
        return unpackedBuffer

    def processFrame(self, unpackedBuffer : str): 
        # print(unpackedBuffer)
        # There is a frame to process. 
        # Decipher and print arguments
        splitBuffer = unpackedBuffer.split(' ')
        commandIndexFromBuffer = int(splitBuffer[0])
        timestamp = int(splitBuffer[1])
        data = splitBuffer[2]
        serialStr = f"CommandIndex: {commandIndexFromBuffer}, t: {timestamp}, d: {data}"
        self._view.updateSerialConsole(serialStr)
        
        self._model.insert(commandIndexFromBuffer, timestamp, data)
        # What to do with different information
        # Begin, standby, Calibrate, reset-IMU, Motor, Help -> Send response to terminal
        # if (commandIndexFromBuffer >= commands.CliCommandIndex.CLI_BEGIN and 
        #     commandIndexFromBuffer <= commands.CliCommandIndex.CLI_HELP):
        #     # Update
        #     # parse for newlines and updates serial console appropriately
        #     # Create objects to retrieve the data from
        #     pass
        # elif (commandIndexFromBuffer == commands.CliCommandIndex.CLI_CONTROL_PACKET):
            
        #     # Parse and store in control packet location
        #     pass
        # elif (commandIndexFromBuffer == commands.CliCommandIndex.CLI_LOCALISATION_PACKET):
        #     # Parse and store in localisation packet location
        #     packet = commands.LocalisationPacket()
        #     packet.fromString(data)
        #     self._model.insertCalibrationPacket(packet, timestamp)
        # elif (commandIndexFromBuffer == commands.CliCommandIndex.CLI_SET_PID):
        #     pass


        