if __name__ == '__main__':
    import sys
    sys.path.append('C:/Users/SamHall/Desktop/Personal/TeensyBreakout/Rosbot/scripts/command_gui')

from comms.packet import Packet, PacketHeader
from comms.packetID import PacketIDs, State, Begin, Standby
from serial import Serial
import struct
# Takes in the header, the buffer and returns a populated data packet
class PacketSerializer:
    identifyingByte = b'\xFF'
    
    @staticmethod
    def serialize(packet : Packet) -> bytes:
       # buffer = bytearray(1)
        # buffer = buffer + PacketSerializer.identifyingByte

        # if (packet.m_header.packetID == PacketIDs.BEGIN):
        #     buffer += struct.pack(Begin.FORMAT, True)
        #     # pass            
        # if (packet.m_header.packetID == PacketIDs.STANDBY):
        #     buffer +=  struct.pack(Standby.FORMAT, True)
        #     # pass
        # if (packet.m_header.packetID == PacketIDs.ESTOP):
        #     pass
        # if (packet.m_header.packetID == PacketIDs.REQUEST):
        #     pass
        # if (packet.m_header.packetID == PacketIDs.STATE):
        #     pass
        # if (packet.m_header.packetID == PacketIDs.ESTIMATE_BIAS):
        #     pass
        # if (packet.m_header.packetID == PacketIDs.LED_CHANGE):
        #     pass

        # # print(buffer)
        # return buffer

    @staticmethod
    def deserialize(buffer : bytes):
        # Read the port, look for the identifying byte
        # header = self.decodeHeader(port)

        # Find identifying byte
        header = PacketHeader()
        if (header.packetID == PacketIDs.STATE):
            unpackedData = struct.unpack(State.FORMAT, buffer)
            stateData = State()
            stateData.eulerXYZ = [unpackedData[0], unpackedData[1], unpackedData[2]]
            stateData.current = [unpackedData[3], unpackedData[4]]
            stateData.velocity = [unpackedData[5], unpackedData[6]]
            print(stateData.toString())


        

    
    @staticmethod
    def decodeHeader(buffer : bytes, packetStart: int) -> PacketHeader:
    
        headerStart = packetStart + 1
        header = PacketHeader()
        res = struct.unpack(PacketHeader.FORMAT, buffer[headerStart:headerStart+PacketHeader.HEADER_SIZE])
        header.packetID = res[0]
        header.packetSize = res[1]
        header.timestamp = res[2]
        print(f"{header.packetID} {header.packetSize} {header.timestamp}")

        # Error checking that packetId is within packet Id range
        
        return header
    
    @staticmethod
    def findIdentifyingByte(buffer : bytes) -> int:
        return buffer.find(PacketSerializer.identifyingByte)

    @staticmethod
    def decodeData(buffer : bytes, header : PacketHeader, packetStart: int):
        dataStart = packetStart + PacketHeader.HEADER_SIZE + 1
        dataEnd = packetStart + header.packetSize + 1
        print(f"Datastart: {dataStart}")
        print(f"Buffer from datastart to dataend: {buffer[dataStart:dataEnd]}")
        print(f"Dataend: {dataEnd}")
        if header.packetID == PacketIDs.STATE:
            print("STATE PACKET")
            res = struct.unpack(State.FORMAT, buffer[dataStart:dataEnd])
            print(res)


def main():
    # 0x42b4    
    byteString = b'\xFF\x05\x00\x00\x00\x28\x00\x00\x00\x12\xa9\x00\x00\x00\x00\xb4\x42\x00\x00\x00\x00\x00\x00\x34\x42\x00\x00\x00\x3F\xCD\xCC\x0C\x3F\x00\x00\x80\x3F\x9A\x99\x99\x3F'
    # Find the start of a packet amongst a sequence 
    packetStartPos = PacketSerializer.findIdentifyingByte(byteString)
    print(f"Identifying byte: {packetStartPos}")
    
    # Decode the header after finding packet start
    header = PacketSerializer.decodeHeader(byteString, packetStartPos)
    print(f"PacketID from header: {header.packetID}")
    PacketSerializer.decodeData(byteString, header, packetStartPos)




if __name__ == "__main__":
    main()
