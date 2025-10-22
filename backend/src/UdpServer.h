#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <memory>
#include <vector>
#include <functional>
#include <mutex>
#include "UdpPacket.h"

class UdpServer
{
public:
    using PacketHandler = std::function<void(const UdpPacket &, const boost::asio::ip::udp::endpoint &)>;

    UdpServer(boost::asio::io_context &io_context, unsigned short port);
    ~UdpServer();

    // 启动服务器
    void start();

    // 停止服务器
    void stop();

    // 注册数据包处理回调
    void setPacketHandler(PacketHandler handler);

    // 广播数据包给所有客户端
    void broadcastPacket(const UdpPacket &packet);

    // 获取连接的客户端数量
    size_t getClientCount() const;

private:
    void startReceive();
    void handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred);
    void handleSend(const boost::system::error_code &error, std::size_t bytes_transferred);
    void addClient(const boost::asio::ip::udp::endpoint &endpoint);
    void removeClient(const boost::asio::ip::udp::endpoint &endpoint);

    boost::asio::io_context &io_context_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    std::array<uint8_t, 1024> recv_buffer_;

    PacketHandler packet_handler_;

    // 客户端列表
    mutable std::mutex clients_mutex_;
    std::vector<boost::asio::ip::udp::endpoint> clients_;

    bool running_;
};

#endif // UDP_SERVER_H
