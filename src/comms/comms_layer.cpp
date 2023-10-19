#include <comms/comms_layer.h>

namespace Rosbot {
/**
 * Take a packet and turn it into a byte stream
 * Assumes buffer has enough memory allocated to it
*/
void PacketSerializer::serialize(Packet packet, byte* buffer) {
    // Memcpy will use native endianness - "little endian"
    const uint32_t dataSize = packet.m_header.m_packetSize - sizeof(PacketHeader);
    buffer[0] = 0xFF;
    memcpy(buffer+1, &packet.m_header, sizeof(PacketHeader));
    memcpy(buffer+sizeof(PacketHeader)+1, packet.m_data, dataSize);
}

/**
 * Creates a packet from a byte stream
*/
void PacketSerializer::deserialize(byte* buffer, uint32_t bufferLength, Packet &packet) {
    // Read sizeof(PacketHeader) bytes from buffer

    // Find the identifying byte
    int packetStart = findIdentifyingByte(buffer, bufferLength);
    // Copy header into header of packet
    
    decodeHeader(buffer, bufferLength, packetStart, packet);
    decodeData(buffer, bufferLength, packetStart, packet);
    
}

int PacketSerializer::findIdentifyingByte(byte* buffer, uint32_t numbytes) {
    for (int i = 0; i < numbytes; i++) {
        if (buffer[i] == m_identifyingByte) {
            return i;
        }
    }
    return -1; // Byte was not found
}

void PacketSerializer::decodeHeader(byte* buffer, uint32_t bufferLength, uint32_t packetStart, Packet &packet) {
    // packetID
    // packetSize
    // timestamp
    const int headerStart = packetStart + 1;
    memcpy(&packet.m_header, buffer + packetStart, sizeof(PacketHeader));
    // Check packetIDs and sizes are within expected values
    // Compute CRC and checksum
}

/**
 * buffer = byte stream,
 * Bufferlength = length of byte stream
 * packetStart = array index specifying where in the byte stream the identifying byte was found
 * Packet = used as input for packetSize, and has data bytes stored in it 
*/
void PacketSerializer::decodeData(byte* buffer, uint32_t bufferLength, uint32_t packetStart, Packet &packet) {
    
    // m_data is a pointer to an array
    // Confirm size of data is less than or equal to size of array
    int dataStart = packetStart + 1 + sizeof(PacketHeader);
    // How many bytes in datasize
    const int numDataBytes = packet.m_header.m_packetSize - sizeof(PacketHeader);
    
    if (dataStart + numDataBytes >= bufferLength) {
        // Something went wrong
        return;
    }

    // Copy bytes from buffer into packet
    memcpy(packet.m_data, &buffer[dataStart], numDataBytes);
}

// void PacketSerializer::debugPrintByteStream(byte* buffer, uint32_t bufferLength) {
//     for (int i = 0; i < bufferLength; i++) {
//         Serial.print("0x");
//         Serial.print(buffer[i], HEX);
//         Serial.print(" ");
//     }
//     Serial.println(" ");
//     // printf("\n");
// }
}