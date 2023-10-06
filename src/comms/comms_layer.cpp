#include <comms/comms_layer.h>
#include <Arduino.h>

/**
 * Take a packet and turn it into a byte stream
 * Assumes buffer has enough memory allocated to it
*/
void PacketSerializer::serialize(Packet packet, byte* buffer) {
    // Memcpy will use native endianness
    const uint32_t dataSize = packet.m_header.m_packetSize - sizeof(PacketHeader);
    memcpy(buffer, &packet.m_header, sizeof(PacketHeader));
    memcpy(buffer+sizeof(PacketHeader), packet.m_data, dataSize);
}

/**
 * Creates a packet from a byte stream
*/
void PacketSerializer::deserialize(byte* buffer, Packet &packet) {
    // Read sizeof(PacketHeader) bytes from buffer
    memcpy(&packet.m_header, buffer, sizeof(PacketHeader));
    const int dataSize = packet.m_header.m_packetSize - sizeof(PacketHeader);
    // Build correct struct here
    memcpy(packet.m_data, buffer+sizeof(PacketHeader), dataSize);
}

void PacketSerializer::debugPrintByteStream(byte* buffer, uint32_t numBytes) {
    for (int i = 0; i < numBytes; i++) {
        Serial.print("0x");
        Serial.print(buffer[i], HEX);
        Serial.println("");
    }
    // printf("\n");
}
