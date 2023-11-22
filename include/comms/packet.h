#pragma once
#include <cstring>

#define PACKET_DATA_SIZE 128

typedef unsigned char byte;

class PacketHeader {
public:
    int m_packetID;        // What type of information are we accessing         
    unsigned int m_packetSize;  // How large is the size of the packet (including the header)     
    unsigned int m_timestamp;   // When was it sent?
    
    PacketHeader() {};
};

class Packet {
public:    
    PacketHeader m_header;
    byte m_data[PACKET_DATA_SIZE];
    
    Packet() {
        memset(m_data, 0, sizeof(byte)*PACKET_DATA_SIZE);
        m_header.m_packetID = -1;
        m_header.m_packetSize = 0;
        m_header.m_timestamp = 0;
    };
};




