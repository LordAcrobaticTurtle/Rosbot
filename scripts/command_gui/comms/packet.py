import comms.packetID

class PacketHeader:
    FORMAT = 'III' # 3 unsigned ints
    HEADER_SIZE = 12 # bytes
    MAX_PACKET_DATA_SIZE = 128 # MAXIMUM SIZE OF DATA PAYLOAD IN PACKET

    def __init__(self) -> None:
        self.packetID = 0
        self.packetSize = 0
        self.timestamp = 0

    def setParams(self, params: tuple):
        assert(len(params) == len(PacketHeader.FORMAT))
        self.packetID = params[0]
        self.packetSize = params[1]
        self.timestamp = params[2]


# Takes in the header, the buffer and returns a populated data packet
class PacketHandler:
    def handlePacket(self, header : PacketHeader, buffer : bytes):
        pass
        # if (header.packetID == comms.packetID.State):
            # Decode buffer looking for state struct




