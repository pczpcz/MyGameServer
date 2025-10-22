#include "UdpReceiver.h"

UdpReceiver::UdpReceiver(boost::asio::io_context &io_context, unsigned short port, PacketQueue &queue)
    : socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      queue_(queue),
      running_(false)
{
    std::cout << "UDP Receiver initialized on port " << port << std::endl;
}

void UdpReceiver::start()
{
    if (running_)
        return;

    running_ = true;
    startReceive();
    std::cout << "UDP Receiver started" << std::endl;
}

void UdpReceiver::stop()
{
    if (!running_)
        return;

    running_ = false;
    boost::system::error_code ec;
    socket_.close(ec);
    std::cout << "UDP Receiver stopped" << std::endl;
}

void UdpReceiver::startReceive()
{
    if (!running_)
        return;

    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_),
        remote_endpoint_,
        [this](const boost::system::error_code &error, std::size_t bytes_transferred)
        {
            handleReceive(error, bytes_transferred);
        });
}

void UdpReceiver::handleReceive(const boost::system::error_code &error, std::size_t bytes_transferred)
{
    std::cout << "Received " << bytes_transferred << " bytes" << std::endl;
    if (!error && bytes_transferred > 0)
    {
        std::vector<uint8_t> data(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred);
        UdpPacket packet = UdpPacket::deserialize(data, remote_endpoint_);

        if (packet.isValid())
        {
            std::cout << "Received valid packet: " << packet.toString()
                      << " from " << remote_endpoint_.address().to_string()
                      << ":" << remote_endpoint_.port() << std::endl;

            // 将数据包放入队列
            queue_.push(packet);
        }
    }
    else
    {
        std::cout << "Error receiving packet: " << error.message() << std::endl;
    }

    startReceive();
}
