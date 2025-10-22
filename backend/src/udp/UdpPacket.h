#ifndef UDP_PACKET_H
#define UDP_PACKET_H

#include <boost/asio.hpp>
#include <vector>
#include <chrono>
#include <string>

// 简单的UDP数据包结构
struct UdpPacket
{
    uint32_t userId;
    uint8_t opType;
    uint8_t mouseButton;
    uint16_t keyCode;
    int16_t mouseX;
    int16_t mouseY;
    uint64_t timestamp;
    boost::asio::ip::udp::endpoint endpoint;

    std::vector<uint8_t> serialize() const;
    static UdpPacket deserialize(const std::vector<uint8_t> &data, const boost::asio::ip::udp::endpoint &endpoint);
    bool isValid() const;
    std::string toString() const;
};

#endif // UDP_PACKET_H
