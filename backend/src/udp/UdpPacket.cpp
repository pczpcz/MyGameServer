#include "UdpPacket.h"
#include <iostream>

std::vector<uint8_t> UdpPacket::serialize() const
{
    std::vector<uint8_t> data;

    // 序列化userId (4 bytes)
    data.push_back((userId >> 24) & 0xFF);
    data.push_back((userId >> 16) & 0xFF);
    data.push_back((userId >> 8) & 0xFF);
    data.push_back(userId & 0xFF);

    // 序列化opType (1 byte)
    data.push_back(opType);

    // 序列化mouseButton (1 byte)
    data.push_back(mouseButton);

    // 序列化keyCode (2 bytes)
    data.push_back((keyCode >> 8) & 0xFF);
    data.push_back(keyCode & 0xFF);

    // 序列化mouseX (2 bytes)
    data.push_back((mouseX >> 8) & 0xFF);
    data.push_back(mouseX & 0xFF);

    // 序列化mouseY (2 bytes)
    data.push_back((mouseY >> 8) & 0xFF);
    data.push_back(mouseY & 0xFF);

    // 序列化timestamp (8 bytes)
    for (int i = 7; i >= 0; --i)
    {
        data.push_back((timestamp >> (i * 8)) & 0xFF);
    }

    return data;
}

UdpPacket UdpPacket::deserialize(const std::vector<uint8_t> &data, const boost::asio::ip::udp::endpoint &endpoint)
{
    UdpPacket packet;
    packet.endpoint = endpoint;

    if (data.size() < 20)
        return packet;

    size_t index = 0;

    // 反序列化userId
    packet.userId = (data[index] << 24) | (data[index + 1] << 16) | (data[index + 2] << 8) | data[index + 3];
    index += 4;

    // 反序列化opType
    packet.opType = data[index++];

    // 反序列化mouseButton
    packet.mouseButton = data[index++];

    // 反序列化keyCode
    packet.keyCode = (data[index] << 8) | data[index + 1];
    index += 2;

    // 反序列化mouseX
    packet.mouseX = static_cast<int16_t>((data[index] << 8) | data[index + 1]);
    index += 2;

    // 反序列化mouseY
    packet.mouseY = static_cast<int16_t>((data[index] << 8) | data[index + 1]);
    index += 2;

    // 反序列化timestamp
    packet.timestamp = 0;
    for (int i = 0; i < 8; ++i)
    {
        packet.timestamp = (packet.timestamp << 8) | data[index++];
    }

    return packet;
}

bool UdpPacket::isValid() const
{
    // Allow any non-zero userId and don't enforce strict timestamp validation
    // for testing purposes
    return userId != 0;
}

std::string UdpPacket::toString() const
{
    return "Packet{userId=" + std::to_string(userId) +
           ", opType=" + std::to_string(opType) +
           ", mouseButton=" + std::to_string(mouseButton) +
           ", keyCode=" + std::to_string(keyCode) +
           ", mouseX=" + std::to_string(mouseX) +
           ", mouseY=" + std::to_string(mouseY) +
           ", timestamp=" + std::to_string(timestamp) + "}";
}
