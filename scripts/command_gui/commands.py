from enum import IntEnum
from enum import auto

class CliCommandIndex(IntEnum):
    CLI_CLI = 0
    CLI_BEGIN = auto()
    CLI_STANDBY = auto()
    CLI_CALIBRATE = auto()
    CLI_RESET_IMU = auto()
    CLI_MOTOR = auto()
    CLI_HELP = auto()
    CLI_CONTROL_PACKET = auto()
    CLI_LOCALISATION_PACKET = auto()
    CLI_NUM_COMMAND = auto()

commands = [
    "cli",
    "Begin",
    "Standby",
    "Calibrate",
    "Reset-IMU", 
    "Motor",
    "Help",
    "ControlPacket",
    "LocalisationPacket",
    "None"
]

FRAMING_START = "0x2"
FRAMING_END = "0x3"

class vector3D:
    def __init__(self) -> None:
        self.x = 0
        self.y = 0
        self.z = 0

    def fromString(self, buffer : str) -> None:
        # Expected string format -> (%f,%f,%f)
        #                             x, y, z
        # Strip the first and last characters
        # Split by commas
        splitBuffer = buffer.strip("()").split(",")
        self.x = float(splitBuffer[0])
        self.y = float(splitBuffer[1])
        self.z = float(splitBuffer[2])

    def __str__(self) -> str:
        return f"({self.x}, {self.y}, {self.z})"
    
class vector2D:
    def __init__(self) -> None:
        self.v1 = 0
        self.v2 = 0

    def fromString(self, buffer : str) -> None:
        splitBuffer = buffer.strip("()").split(",")
        self.v1 = float(splitBuffer[0])
        self.v2 = float(splitBuffer[1])
    
    def __str__(self) -> str:
        return f"({self.v1}, {self.v2})"

class LocalisationPacket:
    def __init__(self) -> None:
        self.accel = vector3D()
        self.gyroRates = vector3D()
        self.vwheel = vector2D()
        self.orientation = vector3D()
        self.pendulumAngle = 0


    def fromString(self, buffer : str):
        
        splitBuffer = buffer.split("),(")
        splitBuffer[0] = splitBuffer[0].strip("()")

        splitBuffer[len(splitBuffer)-1] = splitBuffer[len(splitBuffer)-1].strip("()")
        self.accel.fromString(splitBuffer[0])
        self.gyroRates.fromString(splitBuffer[1])
        self.orientation.fromString(splitBuffer[2])
        self.vwheel.fromString(splitBuffer[3])
        self.pendulumAngle = float(splitBuffer[4])

    def __str__(self) -> str:
        string = f"{self.accel}\n{self.gyroRates}\n{self.orientation}\n{self.vwheel}\n{self.pendulumAngle}"
        return string

class ControlPacket:
    def __init__(self) -> None:
        pass

    def fromString(self, buffer : str):
        pass

def main():
    print("Program start")
    # buffer = "(0.5,0.6,0.7),(0.8,0.9,1.0),(1.1,1.2,1.3),(2.5,2.6)"
    # packet = LocalisationPacket()
    # packet.fromString(buffer)
    # print(packet)
    print(CliCommandIndex.CLI_LOCALISATION_PACKET)

if __name__ == "__main__":
    main()

