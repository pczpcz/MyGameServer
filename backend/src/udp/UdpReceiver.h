#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <boost/asio.hpp>
#include <iostream>
#include "UdpPacket.h"
#include "UdpPacketQueue.h"

// UDP接收器类
class UdpReceiver
{
public:
    UdpReceiver(boost::asio::io_context &io_context, unsigned short port, PacketQueue &queue);
    ~UdpReceiver() = default;

    void start();
    void stop();

private:
    void startReceive();
    void handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred);

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<uint8_t, 1024> recv_buffer_;
    PacketQueue &queue_;
    bool running_;
};

#endif // UDP_RECEIVER_H
