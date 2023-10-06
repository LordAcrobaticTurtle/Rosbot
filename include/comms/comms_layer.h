#include <comms/packet.h>
#include <Arduino.h>

// Used for converting comms packets into a byte stream
class PacketSerializer {
    public:
    static void serialize(Packet packet, byte* buffer);
    static void deserialize(byte* buffer, Packet &packet);
    static void debugPrintByteStream(byte* buffer, uint32_t numBytes);
};

