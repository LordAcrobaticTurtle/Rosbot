from enum import IntEnum
from enum import auto

class PacketIDs(IntEnum):
    BEGIN = 1  
    STANDBY = auto()
    ESTOP = auto()
    REQUEST = auto()
    STATE = auto()  
    ESTIMATE_BIAS = auto()
    LED_CHANGE = auto()
    DEMO = auto()
    NUM_PACKETS = auto()


class Begin():
   def  __init__(self):
        self.doBegin = False
        # self.format = 

class Standby():
    def __init__(self) -> None:
        self.doStandby = True

class ESTOP():
    def __init__(self) -> None:
        self.doSTOP = True

class State():
    FORMAT = '<fffffff'
    SIZE = 7
    def __init__(self) -> None:
        self.current = [0,0]
        self.velocity = [0,0]
        self.eulerXYZ = [0,0,0]
        
    def toString(self) -> str:
        currString = f"Current (L,R): {self.current}\n"
        velString = f"Velocity (L,R): {self.velocity}\n"
        eulerString = f"Euler (X,Y,Z): {self.eulerXYZ}\n"
        return currString + velString + eulerString


