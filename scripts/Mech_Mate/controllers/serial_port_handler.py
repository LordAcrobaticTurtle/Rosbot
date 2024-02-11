# This can be used to open a new thread and poll 
import serial 
import serial.tools.list_ports
import threading
import ui_elements.ui_serial_console as serialView
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
        print("Controller")
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Connect"] = self.open
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Disconnect"] = self.close
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["Get list"] = self.get_list_of_available_ports
        self._view._callbacks[serialView.SerialConsoleCallbackIndex]["<Return>"] = self.send
        return

    def close(self):
        self.isAppRunning = False
        self.send("Standby")
        if (self._isPortOpen):
            self._t1.join()
            self._openPort.close()
            self._isPortOpen = False
            
        
        return

    def open(self, port : str, baudrate : str):
        with self._serialPortLock:
            if (self._isPortOpen):
                print("There is already one active connection")
                return

            self._view._serialConsoleSettings.updateSerialConsole(f"Opening {port} @ {baudrate}")

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
                self._view._serialConsoleSettings.updateSerialConsole(successStr)
            except serial.SerialException as e:
                self._isPortOpen = False
                errorStr = f"Warning: {port} is not accessible - {e}"
                print(errorStr)
                self._view._serialConsoleSettings.updateSerialConsole(errorStr)

    def get_list_of_available_ports (self) -> list:
        ports = serial.tools.list_ports.comports()
        comPortList = [com[0] for com in ports]
        comPortList.insert(len(comPortList), "Mock Connection")
        return comPortList

    def threaded_update(self) -> None:
        buffer = str()
        while (self.isAppRunning):
            
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
        print(string)
        if (self._isPortOpen):
            self._serialPortLock.acquire()
            self._openPort.write((string + "\n").encode())
            self._serialPortLock.release()
            print("String sent!")


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
        self._view._serialConsoleSettings.updateSerialConsole(serialStr)
        
        data = data[1:len(data)-1] # Remove start and end square brackets
        payload = data.split(',')

        self._model.insert(commandIndexFromBuffer, timestamp, payload)
        


        