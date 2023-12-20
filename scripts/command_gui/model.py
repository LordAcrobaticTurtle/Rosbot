
from commands import LocalisationPacket, ControlPacket
import threading

class Model():
    def __init__(self):
        self.name = "Model"
        self.controlPackets = []
        self.calibrationPackets = []
        self.filePath = str()
        self.maxElementsInArray = 200
        self.calibrationPacketsLock = threading.Lock()
        
    
    def insertCalibrationPacket(self, packet : LocalisationPacket, timestamp : float):
        # append to the list
        # If exceeding x number of characters, remove the first element
        dictionary = {
            "data" : packet,
            "timestamp" : timestamp
        }

        with self.calibrationPacketsLock:    
            self.calibrationPackets.append(dictionary)
            if (len(self.calibrationPackets) > self.maxElementsInArray):
                del self.calibrationPackets[0]
    
    def getCalibrationPackets(self) -> list:
        with self.calibrationPacketsLock:
            return self.calibrationPackets.copy()

    def insertControlPacket(self, packet : ControlPacket):
        self.controlPackets.append(packet)    
        if (len(self.controlPackets) > self.maxElementsInArray):
            del self.controlPackets[0]


