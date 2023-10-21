#pragma once
#include <comms/packet.h>
// #include <Arduino.h>
namespace Rosbot {
    typedef unsigned long uint32_t;
    // Used for converting comms packets into a byte stream
    class PacketSerializer {
        public:
            static const byte m_identifyingByte = 0xFF;
            static void serialize(Packet packet, byte* buffer);
            static void deserialize(byte* buffer, uint32_t numBytes, Packet &packet);
            static int findIdentifyingByte(byte* buffer, uint32_t numBytes);
            static void decodeHeader(byte* buffer, uint32_t numBytes, uint32_t packetStart, Packet &packet);
            static void decodeData(byte* buffer, uint32_t bufferLength, uint32_t packetStart, Packet &packet);
            static void debugPrintByteStream(byte* buffer, uint32_t numBytes);
    };
}
