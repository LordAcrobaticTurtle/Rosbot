#include <comms/packet_serializer.h>
#include <comms/packetID.h>
/**
 * Take a packet and turn it into a byte stream
 * Assumes buffer has enough memory allocated to it
*/
void PacketSerializer::serialize(Packet packet, byte* buffer) {
    // Memcpy will use native endianness - "little endian"
    const size_t dataSize = packet.m_header.m_packetSize - sizeof(PacketHeader);
    buffer[0] = 0xFF;
    memcpy(buffer+1, &packet.m_header, sizeof(PacketHeader));
    memcpy(buffer+sizeof(PacketHeader)+1, packet.m_data, dataSize);
}

/**
 * Creates a packet from a byte stream
*/
void PacketSerializer::deserialize(byte* buffer, size_t bufferLength, Packet &packet) {
    // Read sizeof(PacketHeader) bytes from buffer

    // Find the identifying byte

    // There could be more than one identifying byte
    // Assume there won't be more than 256 identifying bytes in a stream
    int identifyingBytes[256];
    memset(identifyingBytes, 0, sizeof(int)*256);

    int numIDBytes = findIdentifyingByte(buffer, bufferLength, identifyingBytes);
    
    int packetStart = decodeHeader(buffer, bufferLength, identifyingBytes, numIDBytes, packet);
    decodeData(buffer, bufferLength, packetStart, packet);
    
}

int PacketSerializer::findIdentifyingByte(byte* buffer, size_t numbytes, int* identifyingBytes) {
    
    int idBytesCounter = 0;
    for (int i = 0; i < numbytes; i++) {
        if (buffer[i] == m_identifyingByte) {
            identifyingBytes[idBytesCounter] = i;
            idBytesCounter++;
        }
    }
    return idBytesCounter;
}
/**
 * @brief Uses the identifying bytes found and decodes a header object from the byte stream.
 * Checks for validity by comparing ID to list of known packetIDs. When a packet is found, the packet.m_header 
 * object will be populated with ID, size and timestamp. If not found, those fields will be set to -1.
 * Packetsize and timestamp will underflow though.
 * @return Returns the offset from the start of the byte stream of the identifying byte of the valid packet.
 * 
*/
int PacketSerializer::decodeHeader(
    byte* buffer, size_t bufferLength, int* identifyingBytes, int numIdBytesFound, Packet &packet
) {
    int i = 0;
    Packet pkt;
    pkt.m_header.m_packetID = -1;
    pkt.m_header.m_packetSize = -1;
    pkt.m_header.m_timestamp = -1;
    
    bool validHeader = false;
    while (i < numIdBytesFound && !validHeader) {
        byte packetStart = identifyingBytes[i];
        byte headerStart = packetStart + 1;
        memcpy(&pkt.m_header, buffer+headerStart, sizeof(PacketHeader));
        validHeader = isHeaderValid(pkt);
        i++;
    }

    packet = pkt;
    return identifyingBytes[i];
    // Compute CRC and checksum
}

/**
 * This function is used to check that the packet passed in is within expected bounds. 
 * I.e, packetID is between 1 and NUM_PACKETS
*/
bool PacketSerializer::isHeaderValid(Packet &packet) {
    int id = packet.m_header.m_packetID;
    
    if (id < PacketID::BEGIN || PacketID::NUM_PACKET_IDS < id) {
        return false;
    }
    // From here we know the packet ID is valid and can choose an expected size to compare against
    //  However this will need to be extended every time I create a new packet
    // unsigned int size = packet.m_header.m_packetSize;
    return true;
}

/**
 * buffer = byte stream,
 * Bufferlength = length of byte stream
 * packetStart = array index specifying where in the byte stream the identifying byte was found
 * Packet = used as input for packetSize, and has data bytes stored in it 
*/
void PacketSerializer::decodeData(byte* buffer, size_t bufferLength, size_t packetStart, Packet &packet) {
    
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

void PacketSerializer::debugPrintByteStream(byte* buffer, size_t bufferLength) {
    for (int i = 0; i < bufferLength; i++) {
        printf("0x");
        printf("%x", buffer[i]);
        printf(" ");
    }
    printf("\n");
    // printf("\n");
}
