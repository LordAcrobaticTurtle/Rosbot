#pragma once
#include <comms/packet.h>

// Used for converting comms packets into a byte stream
class PacketSerializer {
public:
    static void serialize(Packet packet, byte* buffer);
    static void deserialize(byte* buffer, size_t numBytes, Packet &packet);
    static void debugPrintByteStream(byte* buffer, size_t numBytes);

    // Step 1
    static int findIdentifyingByte(byte* buffer, size_t numBytes, int* identifyingBytes);
    // Step 2
    static int decodeHeader(
        byte* buffer, size_t bufferLength, int* identifyingBytes, int numIdBytesFound, Packet &packet
    );
    // Step 3
    static bool isHeaderValid(Packet &packet);
    // Step 4
    static void decodeData(byte* buffer, size_t bufferLength, size_t packetStart, Packet &packet);

    
    static const byte m_identifyingByte = 0xFF;
};

