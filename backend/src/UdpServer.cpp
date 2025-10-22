#include "UdpServer.h"
#include <iostream>
#include <algorithm>

UdpServer::UdpServer(boost::asio::io_context &io_context, unsigned short port)
    : io_context_(io_context),
      socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      running_(false)
{
    std::cout << "UDP Server initialized on port " << port << std::endl;
}

UdpServer::~UdpServer()
{
    stop();
}

void UdpServer::start()
{
    if (running_)
    {
        return;
    }

    running_ = true;
    startReceive();
    std::cout << "UDP Server started" << std::endl;
}

void UdpServer::stop()
{
    if (!running_)
    {
        return;
    }

    running_ = false;
    boost::system::error_code ec;
    socket_.close(ec);

    if (ec)
    {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        clients_.clear();
    }

    std::cout << "UDP Server stopped" << std::endl;
}

void UdpServer::setPacketHandler(PacketHandler handler)
{
    packet_handler_ = handler;
}

void UdpServer::broadcastPacket(const UdpPacket &packet)
{
    if (!running_)
    {
        return;
    }

    auto data = packet.serialize();

    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (const auto &client : clients_)
    {
        socket_.async_send_to(
            boost::asio::buffer(data),
            client,
            boost::bind(&UdpServer::handleSend, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }
}

size_t UdpServer::getClientCount() const
{
    std::lock_guard<std::mutex> lock(clients_mutex_);
    return clients_.size();
}

void UdpServer::startReceive()
{
    if (!running_)
    {
        return;
    }

    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        boost::bind(&UdpServer::handleReceive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void UdpServer::handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (!error && bytes_transferred > 0)
    {
        // 添加客户端到列表
        addClient(remote_endpoint_);

        // 解析数据包
        std::vector<uint8_t> data(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred);
        UdpPacket packet = UdpPacket::deserialize(data, remote_endpoint_);

        if (packet.isValid())
        {
            std::cout << "Received valid packet: " << packet.toString()
                      << " from " << remote_endpoint_.address().to_string()
                      << ":" << remote_endpoint_.port() << std::endl;

            // 调用数据包处理回调
            if (packet_handler_)
            {
                packet_handler_(packet, remote_endpoint_);
            }

            // 广播给所有客户端
            broadcastPacket(packet);
        }
        else
        {
            std::cout << "Received invalid packet from "
                      << remote_endpoint_.address().to_string()
                      << ":" << remote_endpoint_.port() << std::endl;
        }
    }
    else if (error)
    {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }

    // 继续接收下一个数据包
    startReceive();
}

void UdpServer::handleSend(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    if (error)
    {
        std::cerr << "Send error: " << error.message() << std::endl;
    }
}

void UdpServer::addClient(const boost::asio::ip::udp::endpoint &endpoint)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    // 检查是否已经存在
    auto it = std::find(clients_.begin(), clients_.end(), endpoint);
    if (it == clients_.end())
    {
        clients_.push_back(endpoint);
        std::cout << "New client connected: " << endpoint.address().to_string()
                  << ":" << endpoint.port() << std::endl;
        std::cout << "Total clients: " << clients_.size() << std::endl;
    }
}

void UdpServer::removeClient(const boost::asio::ip::udp::endpoint &endpoint)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    auto it = std::find(clients_.begin(), clients_.end(), endpoint);
    if (it != clients_.end())
    {
        clients_.erase(it);
        std::cout << "Client disconnected: " << endpoint.address().to_string()
                  << ":" << endpoint.port() << std::endl;
        std::cout << "Total clients: " << clients_.size() << std::endl;
    }
}
