#include <gtest/gtest.h>
#include <comms/comms_layer.h>
#include <comms/packetID.h>
#include <comms/packet.h>

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
byte encodedBuffer[] = { 0xFF, 0x01, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00, 0xAD, 0x00, 0x00, 0x00};

/**
 * @brief The serialize function should implant an identifying byte 0xFF (at time of writing),
 * following by the packet header, then the packet data
*/
TEST_F(CommsTest, test_packet_header_encode) {
    Packet pkt;
    pkt.m_header.m_packetID = PacketID::BEGIN;
    pkt.m_header.m_packetSize = 0xDE;
    pkt.m_header.m_timestamp = 0xAD;
    memset(pkt.m_data, 0, 128);
    byte buffer[256];
    Rosbot::PacketSerializer::serialize(pkt, buffer);
    
    for (int i = 0; i < sizeof(PacketHeader) + 1; i++ ){
        EXPECT_EQ(buffer[i], encodedBuffer[i]);
    }
}

TEST_F(CommsTest, test_packet_encode_decode) {
    Packet pkt;
    commsPacket::State state;
    state.current = {1.0, 1.5};
    state.eulerXYZ = {90.0, 45.0, 180.0};
    state.velocity = {5.0, 4.5};
}