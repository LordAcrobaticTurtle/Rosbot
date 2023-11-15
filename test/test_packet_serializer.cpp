#include <gtest/gtest.h>
#include "comms/packet_serializer.h"
#include "comms/packet.h"
#include "comms/packetID.h"

class CommsTest : public ::testing::Test {
protected:
    void SetUp() {
        
    }

    void TearDown() {

    }
};


/**
 * An encoded header
*/
// Remember this is all packed in little endian format
// 13 bytes
byte encodedHeader[13] = { 0xFF, 0x01, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xAD, 0x00, 0x00, 0x00};

/*
Cases in this buffer
1.Noise prior to packet
2. Incorrect identifying byte
44 bytes in total
*/
byte encodedHeaderAndDataWithNoise[44] = {
    0x01, 0x02, 0xFF, 0xFF, // Noise, Noise, Noise, start byte
    0x05, 0x00, 0x00, 0x00,  // PacketID = 5, STATE PACKET
    0x28, 0x00, 0x00, 0x00,  // 28 + 12 = 40 = 0x28
    0xAD, 0x00, 0x00, 0x00,   // Timestamp irrelevant in this case /**| END HEADER |*/,
    0x3F, 0x99, 0x99, 0x9A, // 1.2             
    0x3F, 0xB3, 0x33, 0x33, // 1.4
    0x3F, 0x80, 0x00, 0x00, // 1.0
    0x3F, 0x00, 0x00, 0x00, // 0.5
    0x3F, 0x00, 0x00, 0x00, // 0.5
    0x3F, 0x19, 0x99, 0x9A, // 0.6
    0x3F, 0x19, 0x99, 0x9A, // 0.6
};

/*
Cases in this buffer
1.Noise prior to packet
2. Incorrect identifying byte
41 bytes in total
*/
byte encodedHeaderAndData[41] = {
    0xFF,                    // start byte
    0x05, 0x00, 0x00, 0x00,  // PacketID = 5, STATE PACKET
    0x28, 0x00, 0x00, 0x00,  // 28 + 12 = 40 = 0x28
    0xAD, 0x00, 0x00, 0x00,   // Timestamp irrelevant in this case /**| END HEADER |*/,
    0x9A, 0x99, 0x99, 0x3F, // 1.2             
    0x33, 0x33, 0xB3, 0x3F, // 1.4
    0x00, 0x00, 0x80, 0x3F, // 1.0
    0x00, 0x00, 0x00, 0x3F, // 0.5
    0x00, 0x00, 0x00, 0x3F, // 0.5
    0x9A, 0x99, 0x19, 0x3F, // 0.6
    0x9A, 0x99, 0x19, 0x3F, // 0.6
};

byte byteStreamNoIdentifyingBytes[] = {
    0x05, 0x00, 0x00, 0x00, 
    0xDE, 0x00, 0x00, 0x00, 
    0xAD, 0x00, 0x00, 0x00, 
    0x3F, 0x99, 0x99, 0x9A, 
    0x3F, 0xB3, 0x33, 0x33, 
    0x3F, 0x80, 0x00, 0x00, 
    0x3F, 0x00, 0x00, 0x00, 
    0x3F, 0x00, 0x00, 0x00, 
    0x3F, 0x19, 0x99, 0x9A, 
    0x3F, 0x19, 0x99, 0x9A, 
};

TEST_F(CommsTest, test_packet_deserialize_packet) {
    // Test the packet can be deciphered correctly
    Packet packet;
    PacketSerializer::deserialize(encodedHeaderAndDataWithNoise, 45, packet);
    EXPECT_EQ(packet.m_header.m_packetID, PacketID::STATE);
    EXPECT_EQ(packet.m_header.m_packetSize, 0x28);
    EXPECT_EQ(packet.m_header.m_timestamp, 0xAD);
}

/**
 * findIdentifyingBytes should find all identifying bytes in a stream and store them in an array
 * The array must be sized appropriately by the user
*/
TEST_F(CommsTest, test_find_identifying_bytes) {
    int array[256];
    memset(array, 0, sizeof(int)*256);
    int numBytesFound = PacketSerializer::findIdentifyingByte(encodedHeaderAndDataWithNoise, 44, array);
    EXPECT_EQ(numBytesFound, 2);
    EXPECT_EQ(array[0], 2);
    EXPECT_EQ(array[1], 3);
}

TEST_F(CommsTest, test_findIdentifyingBytes_empty_case) {
    int array[256];
    memset(array, 0, sizeof(int)*256);
    int numBytesFound = PacketSerializer::findIdentifyingByte(byteStreamNoIdentifyingBytes, 40, array);
    EXPECT_EQ(numBytesFound, 0);
}

TEST_F(CommsTest, test_decodeHeader_valid_case) {
    int array[256];
    memset(array, 0, sizeof(int)*256);

    int numBytesFound = PacketSerializer::findIdentifyingByte(encodedHeader, 13, array);

    Packet packet;
    PacketSerializer::decodeHeader(encodedHeader, 13, array, numBytesFound, packet);

    EXPECT_EQ(packet.m_header.m_packetID, 0x01);
    EXPECT_EQ(packet.m_header.m_packetSize, 0xDE);
    EXPECT_EQ(packet.m_header.m_timestamp, 0xAD);
}

// No identifying Bytes in the stream
TEST_F(CommsTest, test_decodeHeader_invalid_case) {
    // Put in id bytes without valid packetIDs affter

}

TEST_F(CommsTest, test_decodeHeader_multiple_id_bytes) {
    int array[256];
    memset(array, 0, sizeof(int)*256);

    int numBytesFound = PacketSerializer::findIdentifyingByte(encodedHeaderAndDataWithNoise, 44, array);

    Packet packet;
    PacketSerializer::decodeHeader(encodedHeaderAndDataWithNoise, 44, array, numBytesFound, packet);

    EXPECT_EQ(packet.m_header.m_packetID, 0x05);
    EXPECT_EQ(packet.m_header.m_packetSize, 0x28);
    EXPECT_EQ(packet.m_header.m_timestamp, 0xAD);
}


/**
 * @brief The serialize function should implant an identifying byte, 0xFF (at time of writing),
 * following by the packet header, then the packet data
*/
TEST_F(CommsTest, test_packet_header_serialize) {
    Packet pkt;
    pkt.m_header.m_packetID = PacketID::BEGIN;
    pkt.m_header.m_packetSize = 0xDE;
    pkt.m_header.m_timestamp = 0xAD;
    memset(pkt.m_data, 0, 128);
    byte buffer[256];
    PacketSerializer::serialize(pkt, buffer);
    
    for (int i = 0; i < sizeof(PacketHeader) + 1; i++ ){
        EXPECT_EQ(buffer[i], encodedHeader[i]);
    }
}


TEST_F(CommsTest, test_packet_serialize) {
    Packet pkt;
    pkt.m_header.m_packetID = PacketID::STATE;
    pkt.m_header.m_packetSize = sizeof(PacketHeader) + sizeof(commsPacket::State);
    pkt.m_header.m_timestamp = 0xAD;
    commsPacket::State state;
    state.eulerXYZ[0] = 1.2;
    state.eulerXYZ[1] = 1.4;
    state.eulerXYZ[2] = 1.0;
    state.current[0] = 0.5;
    state.current[1] = 0.5;
    state.velocity[0] = 0.6;
    state.velocity[1] = 0.6;

    memcpy(pkt.m_data, &state, sizeof(commsPacket::State));

    byte array[256];

    PacketSerializer::serialize(pkt, array);

    for (int i = 0; i < 41; i++) {
        // printf("%d) array[]: %d, test[i]: %d\n", i, array[i], encodedHeaderAndData[i]);
        EXPECT_EQ(array[i], encodedHeaderAndData[i]);
    }
}
