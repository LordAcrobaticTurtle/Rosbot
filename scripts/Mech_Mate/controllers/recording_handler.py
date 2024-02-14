# Records control / localisation data to a file as csv

# This should class should handle
# 1. Open a thread
# 2. Open a file, allow caller to decide name of file
# 3. Close thread
# 4. Close file
# 5. Receive data as CSV string. Should be registered as a callback to the model insert
#       However, these callbacks

import view
import model
import threading
from datetime import datetime
from ui_elements.ui_record_settings import RecordSettings

class RecordingHandler():
    def __init__(self, ui : view.View, data : model.Model) -> None:
        self._ui = ui
        self._threadLock = threading.Lock()
        self._model = data
        self._isRecording = False

    def populate_callbacks(self):
        self._ui._recordingSettings._callbacks["start"] = self.startRecording
        self._ui._recordingSettings._callbacks["stop"] = self.stopRecording

    def openFile(self, packetId : int, directoryPath : str):
        if not directoryPath:
            directoryPath = "C:/Users/Sam/Documents/PlatformIO/Projects/Rosbot/datasets"
        currTime = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        return open(f"{directoryPath}/{currTime}-packet-{packetId}-mbed.csv", "w")

    def closeFile(self):
        with self._threadLock:
            self._openFile.close()
        
        return

    def startRecording(self, packetId : int, directoryPath : str):
        print("Start recording")
        self._openFile = self.openFile(packetId, directoryPath)
        # Subscribe to model packets

        self.structure = {
            "unique_id" : "RecordingFunction",
            "packetId" : packetId,
            "func" : self.subbedFunction
        }
        self._model.registerCallbackFunction(self.structure)
        
        self._isRecording = True
        return

    def stopRecording(self):
        print("Stop recording")
        self.closeFile()
        self._model.deRegisterCallbackFunction(self.structure["unique_id"], self.structure["packetId"])
        self._isRecording = False
        return

    def subbedFunction(self, timestamp : int, data : list):
        buffer = f"{timestamp},"
        # For every column in the payload add it to the write buffer
        for value in data:
            buffer += f"{value},"

        buffer += "\n"
        with self._threadLock:
            self._openFile.write(buffer)

        return
        
