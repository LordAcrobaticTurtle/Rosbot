
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
        self.calibrationCallbacks = []
        
    
    def insertCalibrationPacket(self, packet : LocalisationPacket, timestamp : float):
        # append to the list
        # If exceeding x number of characters, remove the first element
        dictionary = {
            "data" : packet,
            "timestamp" : timestamp
        }
        # print(packet)
        with self.calibrationPacketsLock:    
            self.calibrationPackets.append(dictionary)
            if (len(self.calibrationPackets) > self.maxElementsInArray):
                del self.calibrationPackets[0]
        
        self.executeCalibrationCallbackFunctions(timestamp, packet)

        
    
    def getCalibrationPackets(self) -> list:
        with self.calibrationPacketsLock:
            return self.calibrationPackets.copy()

    def insertControlPacket(self, packet : ControlPacket):
        self.controlPackets.append(packet)    
        if (len(self.controlPackets) > self.maxElementsInArray):
            del self.controlPackets[0]
    
    # func and IDstructured like so
    # func = {
    # "func" : function
    # "id" : _where did the callback come from
    # }
    def registerCalibrationCallbackFunction(self, funcAndId : dict):
        if (len(funcAndId.keys()) != 2):
            print("registerCalibCallbacks: Dictionary not the right shape")
            return
        
        if ("func" not in funcAndId.keys()):
            print("registerCalibCallbacks: Dict missing func key")
            return
        
        if ("id" not in funcAndId.keys()):
            print("registerCalibrationCallbacks: Dict missing id key")
            return
        listOfIds = [x["id"] for x in self.calibrationCallbacks]
        
        if (funcAndId["id"] in listOfIds):
            print(f"registerCalibrationCallbacks: {funcAndId['id']} already in the list")
            return

        self.calibrationCallbacks.append(funcAndId)

    def deregisterCalibrationCallbackFunction(self, id : str):
        # listOfIds = [x["id"] for x in self.calibrationCallbacks]
        isIdFound = False
        elementIndex = -1
        for i,j in enumerate(self.calibrationCallbacks):
            if j["id"] == id:
                isIdFound = True
                elementIndex = i
            
        if (isIdFound == False):
            print(f"deregisterCalibrationCallback: {id} does not exist in list")

        del self.calibrationCallbacks[i]

    def executeCalibrationCallbackFunctions(self, timestamp : float, newData : LocalisationPacket):
        for funcAndId in self.calibrationCallbacks:
            funcAndId["func"](timestamp, newData)





def demoCallback(timestamp : float, newData : LocalisationPacket):
    print("Neeeeew data, weehoo")
    print(f"{newData}") 

def main():
    model = Model()
    funcAndId = {
        "id": "Test",
        "func" : demoCallback
    }
    model.registerCalibrationCallbackFunction(funcAndId)
    packet = LocalisationPacket()
    buffer = f"(1.1,1.2,1.3),(1.1,1.2,1.3),(1.1,1.2,1.3),(9.1,8.9)"
    packet.fromString(buffer)
    model.insertCalibrationPacket(packet, 0.5)    
    


if __name__ == "__main__":
    main()