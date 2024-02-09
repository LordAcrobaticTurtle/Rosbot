
import commands
import threading


MAX_QUEUE_LENGTH = 200

class Model():
    def __init__(self):
        self.name = "Model"
        self.dataLock = threading.Lock()
        self._data = {}
        self._callbacks = {}
        for i, __ in enumerate(commands.CliCommandIndex):
            self._data[i] = []
            self._callbacks[i] = []
        
    # Assume payload is a csv separable string. 
    def insert(self, packetId : int, timestamp : int, payload : list):
        print("Data inserted")
        
        structure = {
            "timestamp" : timestamp,
            "payload" : payload
        }

        self.dataLock.acquire()

        self._data[packetId].append(structure)
        if len(self._data[packetId]) > MAX_QUEUE_LENGTH:
            del self._data[packetId][0]

        self.dataLock.release()

        self.executeCallbackFunctions(packetId, timestamp, payload)

    def registerCallbackFunction(self, funcAndId : dict):
        if (len(funcAndId.keys()) != 3):
            print("register Callbacks: Dictionary not the right shape")
            return
        
        if ("packetId" not in funcAndId.keys()):
            print("Register Callbacks: Dict missing packetID key")
            return

        if ("func" not in funcAndId.keys()):
            print("register Callbacks: Dict missing func key")
            return
        
        if ("unique_id" not in funcAndId.keys()):
            print("register Callbacks: Dict missing id key")
            return
        
        listOfIds = [x["id"] for x in self._callbacks[funcAndId["packetId"]]]
        
        if (funcAndId["id"] in listOfIds):
            print(f"registerCalibrationCallbacks: {funcAndId['id']} already in the list")
            return

        structure = {
            "id" : funcAndId["id"],
            "func" : funcAndId["func"]
        }

        self._callbacks[funcAndId["packetId"]].append(funcAndId)

    def deRegisterCallbackFunction(self, id : str, packetID : int):
        isIdFound = False
        
        for i,j in enumerate(self._callbacks[packetID]):
            if j["id"] == id:
                isIdFound = True
                elementIndex = i
            
        if (isIdFound == False):
            print(f"deregisterCalibrationCallback: {id} does not exist in list")

        del self._callbacks[packetID][elementIndex]

    def executeCallbackFunctions(self, packetId : int, timestamp : float, newData : list):
        for funcAndId in self._callbacks[packetId]:
            funcAndId["func"](timestamp, newData)

def demoCallback(timestamp : float, newData):
    print("Neeeeew data, weehoo")
    print(f"{newData}") 

def main():
    model = Model()
    funcAndId = {
        "id": "Test",
        "func" : demoCallback
    }

    # for i, packetID in enumerate(commands.CliCommandIndex):
        # print(packetID)

    test = {
        int(commands.CliCommandIndex.CLI_BEGIN) : 0
    }

    test[1] = 1

    print(test)
    # model.registerCalibrationCallbackFunction(funcAndId)
    # packet = 
    # buffer = f"(1.1,1.2,1.3),(1.1,1.2,1.3),(1.1,1.2,1.3),(9.1,8.9)"
    # packet.fromString(buffer)
    # model.insertCalibrationPacket(packet, 0.5)    
    


if __name__ == "__main__":
    main()