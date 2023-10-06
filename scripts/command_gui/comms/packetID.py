from enum import Enum


class PacketIDs(Enum):
    BEGIN = 1   
    STANDBY = 2
    ESTOP = 3
    STATE = 4   
    ESTIMATE_BIAS = 5
    LED_CHANGE = 6 
    DEMO = 7

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
    def __init__(self) -> None:
        self.current = [0,0]
        self.velocity = [0,0]
        self.eulerXYZ = [0,0,0]
        self.format = 'ddddddd'
        
    
    def toString(self) -> str:
        currString = f"Current (L,R): {self.current}\n"
        velString = f"Velocity (L,R): {self.velocity}\n"
        eulerString = f"Euler (X,Y,Z): {self.eulerXYZ}\n"
        return currString + velString + eulerString


